#include "params.h"
#include "backing_store.h"
#include <stdio.h>

void writeOutFrameToStore(int phy_frame, int page) {
    // Check if the frame is dirty
    if (page_table[page].dirty) {
        FILE *backing_store;
        backing_store = fopen("data/Address_store.bin", "rw+");
        if (backing_store == NULL) {
            printf("Failed to open backing_store.bin for writing\n");
            return;
        }
        fseek(backing_store, page_table[page].frame_number * PAGE_SIZE, SEEK_SET);
        fwrite(&physical_memory[phy_frame * PAGE_SIZE], 1, PAGE_SIZE, backing_store);
        fclose(backing_store);

        // Update the dirty bit in the page table and TLB
        page_table[page].dirty = 0;
        for (int i = 0; i < TLB_SIZE; i++) {
            if (trans_look_buffer[i].valid && trans_look_buffer[i].frame_number == phy_frame) {
                trans_look_buffer[i].dirty = 0;
            }
        }
    }
}
// void writeOutFrameToStore(int phy_frame, int store_frame) {
//     FILE *backing_store;
//     backing_store = fopen("Address_store.bin", "rw+");
//     if (backing_store == NULL) {
//         printf("Failed to open backing_store.bin for writing\n");
//         return;
//     }
//     fseek(backing_store, store_frame * PAGE_SIZE, SEEK_SET);
//     fwrite(&physical_memory[phy_frame * PAGE_SIZE], 1, PAGE_SIZE, backing_store);
//     fclose(backing_store);
// }

void readInFrameFromStore(int phy_frame, int store_frame) {
    FILE *backing_store;
    backing_store = fopen("data/Address_store.bin", "r");
    if (backing_store == NULL) {
        printf("Failed to open backing_store.bin for reading\n");
        return;
    }
    fseek(backing_store, store_frame * PAGE_SIZE, SEEK_SET);
    fread(&physical_memory[phy_frame * PAGE_SIZE], 1, PAGE_SIZE, backing_store);
    fclose(backing_store);
}