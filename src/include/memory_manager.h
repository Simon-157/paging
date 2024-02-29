#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "params.h"


typedef struct {
    int frame_number;
    int valid;
    int dirty;
    int lru_counter;
} Page_Table_Entry;

typedef struct {
    int page_number;
    int frame_number;
    int valid;
    int dirty;
} TLB_Entry;


extern Page_Table_Entry page_table[PAGE_TABLE_SIZE];
extern TLB_Entry trans_look_buffer[TLB_SIZE];
extern int lru_counter;
extern char physical_memory[PAGE_SIZE * PHYS_MEM_SIZE];

int getLeastRecentlyUsedFrame();
int getNextFrameForReplacement();
void removeFrameFromPageTable(int frame);
int getFrameFromPageTable(int page, int write_to_mem);
int getFrameFromTLB(int input_page, int write_to_mem);
void updateTLB(int input_page, int input_frame);

#endif /* MEMORY_MANAGEMENT_H */
