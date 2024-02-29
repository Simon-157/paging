#include "memory_manager.h"
#include "backing_store.h"
#include "statistics.h"
#include <stdio.h>


Page_Table_Entry page_table[PAGE_TABLE_SIZE];
TLB_Entry trans_look_buffer[TLB_SIZE];
int lru_counter;
char physical_memory[PAGE_SIZE * PHYS_MEM_SIZE];

int getLeastRecentlyUsedFrame() {
    int frame_found[PHYS_MEM_SIZE] = {0};
    int frame, frame_age;
    int LRU_frame = -1;
    int oldest_age = -1;

    // Check if All Frames are Already in Page Table
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (page_table[i].valid) {
            frame = page_table[i].frame_number;
            frame_found[frame] = 1;
        }
    }

    // If not, return the first frame not already in Page Table
    for (int i = 0; i < PHYS_MEM_SIZE; i++) {
        if (!frame_found[i]) {
            return i;
        }
    }

    // Otherwise, return the Least Recently Used Frame
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (page_table[i].valid) {
            // Extract Frame and LRU Counter from Page Entry
            frame = page_table[i].frame_number;
            int counter = page_table[i].lru_counter;

            // Get the LRU "Age" of the Frame
            if (counter < lru_counter) {
                frame_age = lru_counter - counter;
            } else {
                frame_age = lru_counter + (MAX_LRU_COUNTER - counter);
            }

            // Update Oldest Frame
            if (frame_age > oldest_age) {
                oldest_age = frame_age;
                LRU_frame = frame;
            }
        }
    }

    return LRU_frame;
}

int getNextFrameForReplacement() {
    static int frame = -1;
    if (PHYS_MEM_SIZE == PAGE_TABLE_SIZE) {
        frame = (frame + 1) & 0x000000FF;
    } else {
        frame = getLeastRecentlyUsedFrame();
    }
    return frame;
}

void removeFrameFromPageTable(int frame) {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        // Find Page Table Entry that matches Frame Number
        if (page_table[i].valid && page_table[i].frame_number == frame) {
            // If Page Table Dirty Bit is Set, Write Page Back to Store
            if (page_table[i].dirty) {
                writeOutFrameToStore(frame, i);
                num_dirty_swaps++;
            }
            // Clear the Page Table Entry
            page_table[i].valid = 0;
            page_table[i].dirty = 0;
        }
    }
}

int getFrameFromPageTable(int page, int write_to_mem) {
    int frame = 0;

    // Check if Page is already in Physical Memory
    if (page_table[page].valid) {
        frame = page_table[page].frame_number;
    } else {
        num_page_faults++;
        // If Page Fault, Read in from Backing Store
        frame = getNextFrameForReplacement();
        removeFrameFromPageTable(frame);
        readInFrameFromStore(frame, page);
        // Update Page Table
        page_table[page].valid = 1;
        page_table[page].frame_number = frame;
        // Update Translation Lookaside Buffer (TLB)
        updateTLB(page, frame);
    }
    // Set Dirty Bit on Write
    if (write_to_mem) {
        page_table[page].dirty = 1;
    }
    return frame;
}

int getFrameFromTLB(int input_page, int write_to_mem) {
    // Loop Through TLB
    for (int i = 0; i < TLB_SIZE; i++) {
        // If Page is Found, return the associated Frame Number
        if (trans_look_buffer[i].valid && (trans_look_buffer[i].page_number == input_page)) {
            // Set Dirty Bit on Write
            if (write_to_mem) {
                trans_look_buffer[i].dirty = 1;
            }
            // Update Stats and Return Frame
            num_tlb_hits++;
            return trans_look_buffer[i].frame_number;
        }
    }
    // If Page is not in TLB, Get Frame from Page Table
    return getFrameFromPageTable(input_page, write_to_mem);
}

void updateTLB(int input_page, int input_frame) {
    static int next_reg = 0;

    // Check if Page already exists in TLB (i.e. update to Frame Number only)
    for (int i = 0; i < TLB_SIZE; i++) {
        if (trans_look_buffer[i].valid && (trans_look_buffer[i].page_number == input_page)) {
            trans_look_buffer[i].frame_number = input_frame;
            return;
        }
    }

    // Check if TLB's Old Value has a Dirty Bit
    if (trans_look_buffer[next_reg].valid && trans_look_buffer[next_reg].dirty) {
        writeOutFrameToStore(trans_look_buffer[next_reg].frame_number, trans_look_buffer[next_reg].page_number);
        num_dirty_swaps++;
    }

    // Update TLB's Next Register within its FIFO
    trans_look_buffer[next_reg].valid = 1;
    trans_look_buffer[next_reg].page_number = input_page;
    trans_look_buffer[next_reg].frame_number = input_frame;

    // Update Next Register Value
    next_reg = next_reg + 1;
    if (next_reg >= TLB_SIZE) {
        next_reg = 0;
    }
}