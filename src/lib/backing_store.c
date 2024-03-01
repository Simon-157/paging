#include <stdlib.h>
#include <stdio.h>
#include "params.h"
#include "backing_store.h"


void writeOutFrameToStore(int phy_frame) {
    // Check if the frame is dirty
    if (physical_memory[phy_frame].dirty) {
        FILE *backing_store;
        backing_store = fopen("data/Address_store.bin", "rw+");
        if (backing_store == NULL) {
            printf("Failed to open backing_store.bin for writing\n");
            return;
        }
        fseek(backing_store, physical_memory[phy_frame].page_number * PAGE_SIZE, SEEK_SET);
        fwrite(&physical_memory[phy_frame].page_number, 1, PAGE_SIZE, backing_store);
        fclose(backing_store);

        // Update the dirty bit in the frame and TLB
        physical_memory[phy_frame].dirty = 0;
        for (int i = 0; i < TLB_SIZE; i++) {
            if (trans_look_buffer[i].valid && trans_look_buffer[i].frame_number == phy_frame) {
                trans_look_buffer[i].dirty = 0;
            }
        }
    }
}

void readInFrameFromStore(int frame_number, int page_number) {
    FILE *backing_store;
    backing_store = fopen("data/Address_store.bin", "r");
    if (backing_store == NULL) {
        printf("Failed to open backing_store.bin for reading\n");
        return;
    }
    fseek(backing_store, page_number * PAGE_SIZE, SEEK_SET);
    fread(physical_memory[frame_number].page_contents, 1, PAGE_SIZE, backing_store);
    fclose(backing_store);

    // Update the page number and valid bit in the frame
    physical_memory[frame_number].page_number = page_number;
    physical_memory[frame_number].valid = 1;
}

void loadPageFromBackingStore(int page, char *frame_start, int process_id) {
    // Construct the file name
    char file_name[50];
    sprintf(file_name, "data/Address_store.bin");

    // Open the file
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file %s.\n", file_name);
        exit(-1);
    }

    // Seek to the start of the page
    fseek(file, page * PAGE_SIZE, SEEK_SET);

    // Read the page into the frame
    fread(frame_start, sizeof(char), PAGE_SIZE, file);

    // Close the file
    fclose(file);
}