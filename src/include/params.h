#ifndef MEMORY_PARAMETERS_H
#define MEMORY_PARAMETERS_H

#define PAGE_SIZE            4096    // 4KB page size
#define FRAME_SIZE           4096    // 4KB frame size
#define PAGE_TABLE_SIZE      (PHYS_MEM_SIZE / PAGE_SIZE)    // Number of pages allowed in the page table


#define PHYS_MEM_SIZE        1048576    // 1MB physical memory size

#define TLB_SIZE             32      // 32-entry TLB

#define VIRTUAL_PAGE_MASK    0x0000FF00
#define VIRTUAL_OFFSET_MASK  0x000000FF
#define VIRTUAL_PAGE_SHIFT   8
#define VIRTUAL_OFFSET_SHIFT 0

#define PAGE_ENTRY_VALID_MASK   0x80000000
#define PAGE_DIRTY_BIT_MASK    0x40000000
#define PAGE_LRU_COUNTER_MASK  0x00FFFF00
#define PAGE_FRAME_NUM_MASK    0x000000FF
#define PAGE_ENTRY_VALID_SHIFT  31
#define PAGE_DIRTY_BIT_SHIFT   30
#define PAGE_LRU_COUNTER_SHIFT 8
#define PAGE_FRAME_NUM_SHIFT   0

#define TLB_ENTRY_VALID_MASK   0x80000000
#define TLB_DIRTY_BIT_MASK   0x40000000
#define TLB_PAGE_NUM_MASK    0x0000FF00
#define TLB_FRAME_NUM_MASK   0x000000FF
#define TLB_ENTRY_VALID_SHIFT  31
#define TLB_DIRTY_BIT_SHIFT  30
#define TLB_PAGE_NUM_SHIFT   8
#define TLB_FRAME_NUM_SHIFT  0

#define MAX_LRU_COUNTER   65000
#define PT_LRU_COUNTER_SHIFT 8
#define PT_LRU_COUNTER_MASK  0x00FFFF00

#endif /* MEMORY_PARAMETERS_H */
