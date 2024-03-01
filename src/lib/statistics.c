#include "statistics.h"
#include "params.h"

int num_memory_accesses = 0;
int num_page_faults = 0;
int num_tlb_hits = 0;
int num_memory_reads = 0;
int num_memory_writes = 0;
int num_dirty_swaps = 0;

void printStatistics()
{
    printf("\033[1;32m"); // Set color to green
    printf("+---------------------------------+---------+\n");
    printf("| Metric                          | Value   |\n");
    printf("+---------------------------------+---------+\n");
    printf("\033[0m"); // Reset color

    printf("| Page Table Size (# Page-Entries)       | \033[1;31m%7d\033[0m |\n", PAGE_TABLE_SIZE); // Red
    printf("| Physical Memory Size (# Bytes) | \033[1;33m%7d\033[0m |\n", PHYS_MEM_SIZE); // Yellow
    printf("| TLB Size (# Page-Frame Regs)    | \033[1;34m%7d\033[0m |\n", TLB_SIZE); // Blue
    printf("| Total Num Memory Accesses       | \033[1;32m%7d\033[0m |\n", num_memory_accesses); // Green
    printf("| Num of allocated frames: | \033[1;32m%7d\033[0m |\n", getNumberOfAllocatedFrames());
    printf("| Num of free frames: | \033[1;32m%7d\033[0m |\n", PHYS_MEM_SIZE/FRAME_SIZE - getNumberOfAllocatedFrames());
    printf("| Total Num Reads                 | \033[1;33m%7d\033[0m |\n", num_memory_reads); // Yellow
    printf("| Total Num Writes                | \033[1;34m%7d\033[0m |\n", num_memory_writes); // Blue
    printf("| Total Num Dirty Swaps           | \033[1;35m%7d\033[0m |\n", num_dirty_swaps); // Magenta
    printf("| Total Num Page Faults           | \033[1;35m%7d\033[0m |\n", num_page_faults); // Magenta
    printf("| Total TLB Hits                  | \033[1;36m%7d\033[0m |\n", num_tlb_hits); // Cyan
    printf("| Page Fault Rate                 | \033[1;37m%7.2f\033[0m |\n", (float)num_page_faults / (float)num_memory_accesses); // White
    printf("| TLB Hit Rate                    | \033[1;31m%7.2f\033[0m |\n", (float)num_tlb_hits / (float)num_memory_accesses); // Red


    printf("\033[1;32m"); // Set color to green
    printf("+---------------------------------+---------+\n");
    printf("\033[0m"); // Reset color
}