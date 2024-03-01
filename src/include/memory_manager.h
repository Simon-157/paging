#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "params.h"

typedef struct Page_Table_Entry {
    int frame_number;
    int valid;
    int dirty;
    int lru_counter;
    struct Page_Table_Entry* secondary[PAGE_TABLE_SIZE]; // Pointer to secondary page table
} Page_Table_Entry;

typedef struct {
    int page_number;
    int frame_number;
    int valid;
    int dirty;
    int lru_counter;
} TLB_Entry;

typedef struct {
    int valid;
    char page_contents[PAGE_SIZE];
    int page_number; 
    int dirty;
    int lru_counter;
} Frame;

extern Page_Table_Entry page_table[PAGE_TABLE_SIZE]; // This is now the master page table
extern TLB_Entry trans_look_buffer[TLB_SIZE];
extern int lru_counter;
extern Frame physical_memory[PHYS_MEM_SIZE];
int getLeastRecentlyUsedFrame();
int getNextFrameForReplacement();
void removeFrameFromPageTable(int frame);
int getFrameFromPageTable(int page, int write_to_mem);
int getFrameFromTLB(int input_page, int write_to_mem);
void updateTLB(int input_page, int input_frame);
void addToTLB(int page, int frame);
char accessMemory(int phy_address, int write);;
int loadPageIntoMemory(int page, int process_id);
int getNumberOfAllocatedFrames();

#endif /* MEMORY_MANAGEMENT_H */