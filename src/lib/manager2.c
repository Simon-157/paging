// #include "memory_manager.h"
// #include "backing_store.h"
// #include "statistics.h"
// #include <stdio.h>


// // Page_Table_Entry page_table[PAGE_TABLE_SIZE];
// TLB_Entry trans_look_buffer[TLB_SIZE];
// int lru_counter;
// Frame physical_memory[PHYS_MEM_SIZE];

// Page_Table_Entry page_table[PAGE_TABLE_SIZE][PAGE_TABLE_SIZE];

// void initializePageTable() {
//     for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
//         for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
//             page_table[i][j].valid = 0;
//             page_table[i][j].frame_number = -1;
//             page_table[i][j].dirty = 0;
//             page_table[i][j].lru_counter = 0;
//         }
//     }
// }

// // Function to translate virtual to physical addresses
// int translateVirtualToPhysical(int virtual_address) {
//     int page_number = virtual_address / PAGE_SIZE;
//     int offset = virtual_address % PAGE_SIZE;
//     int outer_index = page_number / PAGE_TABLE_SIZE;
//     int inner_index = page_number % PAGE_TABLE_SIZE;

//     if (!page_table[outer_index][inner_index].valid) {
//         handlePageFault(page_number);
//     }

//     int frame_number = page_table[outer_index][inner_index].frame_number;
//     return frame_number * PAGE_SIZE + offset;
// }



// // Function to allocate a page
// void allocatePage(int page_number, int frame_number, int initialize) {
//     int outer_index = page_number / PAGE_TABLE_SIZE;
//     int inner_index = page_number % PAGE_TABLE_SIZE;

//     page_table[outer_index][inner_index].valid = 1;
//     page_table[outer_index][inner_index].frame_number = frame_number;

//     if (initialize) {
//         for (int i = 0; i < PAGE_SIZE; i++) {
//             physical_memory[frame_number].page_contents[i] = 0;
//         }
//     }
// }

// // Function to deallocate a page
// void deallocatePage(int page_number) {
//     int outer_index = page_number / PAGE_TABLE_SIZE;
//     int inner_index = page_number % PAGE_TABLE_SIZE;

//     if (page_table[outer_index][inner_index].valid) {
//         int frame_number = page_table[outer_index][inner_index].frame_number;
//         physical_memory[frame_number].valid = 0;
//     }

//     page_table[outer_index][inner_index].valid = 0;
// }



// // Function to handle page faults
// void handlePageFault(int page_number) {
//     int frame_number = getLeastRecentlyUsedFrame();
//     if (physical_memory[frame_number].valid) {
//         deallocatePage(physical_memory[frame_number].page_number);
//     }

//     loadPageIntoMemory(page_number, frame_number);
//     allocatePage(page_number, frame_number, 0);
// }

// // Function to track free and allocated frames
// void trackFrames() {
//     printf("Frame Status:\n");
//     for (int i = 0; i < PHYS_MEM_SIZE; i++) {
//         if (physical_memory[i].valid) {
//             printf("Frame %d: Allocated\n", i);
//         } else {
//             printf("Frame %d: Free\n", i);
//         }
//     }
// }
// int getLeastRecentlyUsedFrame() {
//     int frame_found[PHYS_MEM_SIZE] = {0};
//     int frame, frame_age;
//     int LRU_frame = -1;
//     int oldest_age = -1;

//     // Check if All Frames are Already in Page Table
//     for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
//         for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
//             if (page_table[i][j].valid) {
//                 frame = page_table[i][j].frame_number;
//                 frame_found[frame] = 1;
//             }
//         }
//     }

//     // If not, return the first frame not already in Page Table
//     for (int i = 0; i < PHYS_MEM_SIZE; i++) {
//         if (!frame_found[i]) {
//             return i;
//         }
//     }

//     // Otherwise, return the Least Recently Used Frame
//     for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
//         for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
//             if (page_table[i][j].valid) {
//                 // Extract Frame and LRU Counter from Page Entry
//                 frame = page_table[i][j].frame_number;
//                 int counter = page_table[i][j].lru_counter;

//                 // Get the LRU "Age" of the Frame
//                 if (counter < lru_counter) {
//                     frame_age = lru_counter - counter;
//                 } else {
//                     frame_age = lru_counter + (MAX_LRU_COUNTER - counter);
//                 }

//                 // Update Oldest Frame
//                 if (frame_age > oldest_age) {
//                     oldest_age = frame_age;
//                     LRU_frame = frame;
//                 }
//             }
//         }
//     }

//     return LRU_frame;
// }

// void removeFrameFromPageTable(int frame) {
//     for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
//         for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
//             // Find Page Table Entry that matches Frame Number
//             if (page_table[i][j].valid && page_table[i][j].frame_number == frame) {
//                 // If Page Table Dirty Bit is Set, Write Page Back to Store
//                 if (page_table[i][j].dirty) {
//                     writeOutFrameToStore(frame);
//                     num_dirty_swaps++;
//                 }
//                 // Clear the Page Table Entry
//                 page_table[i][j].valid = 0;
//                 page_table[i][j].dirty = 0;
//             }
//         }
//     }
// }

// int getFrameFromPageTable(int page, int write_to_mem) {
//     int frame = 0;
//     int outer_index = page / PAGE_TABLE_SIZE;
//     int inner_index = page % PAGE_TABLE_SIZE;

//     // Check if Page is already in Physical Memory
//     if (page_table[outer_index][inner_index].valid) {
//         frame = page_table[outer_index][inner_index].frame_number;
//     } else {
//         num_page_faults++;
//         // If Page Fault, Read in from Backing Store
//         frame = getNextFrameForReplacement();
//         removeFrameFromPageTable(frame);
//         readInFrameFromStore(frame, page);
//         // Update Page Table
//         page_table[outer_index][inner_index].valid = 1;
//         page_table[outer_index][inner_index].frame_number = frame;
//         // Update Translation Lookaside Buffer (TLB)
//         updateTLB(page, frame);
//     }
//     // Set Dirty Bit on Write
//     if (write_to_mem) {
//         page_table[outer_index][inner_index].dirty = 1;
//     }
//     return frame;
// }


// int getFrameFromTLB(int input_page, int write_to_mem) {
//     // Loop Through TLB
//     for (int i = 0; i < TLB_SIZE; i++) {
//         // If Page is Found, return the associated Frame Number
//         if (trans_look_buffer[i].valid && (trans_look_buffer[i].page_number == input_page)) {
//             // Set Dirty Bit on Write
//             if (write_to_mem) {
//                 trans_look_buffer[i].dirty = 1;
//             }
//             // Update Stats and Return Frame
//             num_tlb_hits++;
//             return trans_look_buffer[i].frame_number;
//         }
//     }
//     // If Page is not in TLB, Get Frame from Page Table
//     return getFrameFromPageTable(input_page, write_to_mem);
// }

// void updateTLB(int input_page, int input_frame) {
//     static int next_reg = 0;

//     // Check if Page already exists in TLB (i.e. update to Frame Number only)
//     for (int i = 0; i < TLB_SIZE; i++) {
//         if (trans_look_buffer[i].valid && (trans_look_buffer[i].page_number == input_page)) {
//             trans_look_buffer[i].frame_number = input_frame;
//             return;
//         }
//     }

//     // Check if TLB's Old Value has a Dirty Bit
//     if (trans_look_buffer[next_reg].valid && trans_look_buffer[next_reg].dirty) {
//         writeOutFrameToStore(trans_look_buffer[next_reg].frame_number);
//         num_dirty_swaps++;
//     }

//     // Update TLB's Next Register within its FIFO
//     trans_look_buffer[next_reg].valid = 1;
//     trans_look_buffer[next_reg].page_number = input_page;
//     trans_look_buffer[next_reg].frame_number = input_frame;

//     // Update Next Register Value
//     next_reg = next_reg + 1;
//     if (next_reg >= TLB_SIZE) {
//         next_reg = 0;
//     }
// }


// void addToTLB(int page, int frame) {
//     // Find an empty slot in the TLB for the new entry
//     for (int i = 0; i < TLB_SIZE; i++) {
//         if (trans_look_buffer[i].valid == 0) {
//             trans_look_buffer[i].page_number = page;
//             trans_look_buffer[i].frame_number = frame;
//             trans_look_buffer[i].valid = 1;
//             return;
//         }
//     }

//     // If no empty slot is found, replace the oldest entry
//     int oldest_entry = 0;
//     for (int i = 1; i < TLB_SIZE; i++) {
//         if (trans_look_buffer[i].lru_counter < trans_look_buffer[oldest_entry].lru_counter) {
//             oldest_entry = i;
//         }
//     }
//     trans_look_buffer[oldest_entry].page_number = page;
//     trans_look_buffer[oldest_entry].frame_number = frame;
//     trans_look_buffer[oldest_entry].valid = 1;
// }

// char accessMemory(int physical_address, int write) {
//     int frame = physical_address / PAGE_SIZE;
//     int offset = physical_address % PAGE_SIZE;
//     int outer_index = physical_memory[frame].page_number / PAGE_TABLE_SIZE;
//     int inner_index = physical_memory[frame].page_number % PAGE_TABLE_SIZE;

//     if (write) {
//         // If it's a write operation, mark the page as dirty
//         physical_memory[frame].dirty = 1;

//         // Update the TLB
//         updateTLB(physical_memory[frame].page_number, frame);

//         // Update the page table
//         page_table[outer_index][inner_index].dirty = 1;

//         // Update the LRU
//         physical_memory[frame].lru_counter = lru_counter++;
//         num_memory_reads++;
//     }
//     else {
//         // Update the TLB
//         updateTLB(physical_memory[frame].page_number, frame);

//         // Update the LRU
//         physical_memory[frame].lru_counter = lru_counter++;
//         num_memory_reads++;
//     }

//     // Access the memory

//     return physical_memory[frame].page_contents[offset]; 
// }


// int loadPageIntoMemory(int page, int process_id) {
//     // Find a frame to load the page into
//     int frame = getNextFrameForReplacement();
//     int outer_index = page / PAGE_TABLE_SIZE;
//     int inner_index = page % PAGE_TABLE_SIZE;

//     // If the frame is currently being used, remove it from the page table
//     if (physical_memory[frame].valid) { 
//         removeFrameFromPageTable(frame);
//     }

//     // Load the page into the frame
//     char *frame_start = physical_memory[frame].page_contents; 
//     loadPageFromBackingStore(page, frame_start, process_id);

//     // Update the page table
//     page_table[outer_index][inner_index].frame_number = frame;
//     page_table[outer_index][inner_index].valid = 1;
//     page_table[outer_index][inner_index].dirty = 0;
//     page_table[outer_index][inner_index].lru_counter = lru_counter++;

//     // Update the physical memory
//     physical_memory[frame].valid = 1; 
//     physical_memory[frame].dirty = 0; 
//     physical_memory[frame].lru_counter = lru_counter++; 
//     physical_memory[frame].page_number = page;

//     return frame;
// }

// int getNumberOfAllocatedFrames (){
//     int num_allocated_frames = 0;
//     for (int i = 0; i < PHYS_MEM_SIZE; i++) {
//         if (physical_memory[i].valid) {
//             num_allocated_frames++;
//         }
//     }
//     return num_allocated_frames;}