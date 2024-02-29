#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "params.h"
#include "statistics.h"
#include "memory_manager.h"
#include "virtual_memory.h"
#include "backing_store.h"

#define RANDOM_ADDRESSES "data/random_addresses.txt"

void simulateMemoryAccess(int process_size);
void printStatistics();
int main()
{
    srand(time(NULL));

    // Simulate processes accessing memory with varying sizes
    for (int i = 0; i < 5; ++i)
    {
        int process_size = (rand() % 5 + 1) * PAGE_SIZE; // Vary process sizes (1 to 5 pages)
        simulateMemoryAccess(process_size);
    }

    // Print Statistics
    printStatistics();

    return 0;
}

void simulateMemoryAccess(int process_size)
{
    FILE *file = fopen(RANDOM_ADDRESSES, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *ptr;
        int log_address = strtol(line, &ptr, 10);
        char type = *ptr;

        int page = getPageNumber(log_address);
        int offset = getOffset(log_address);

        // Check TLB
        int frame = getFrameFromTLB(page, type == 'W'); // Simulate a write or read to memory based on type

        // Check Page Table
        if (frame < 0)
        {
            frame = getFrameFromPageTable(page, type == 'W'); // Simulate a write or read to memory based on type
        }

        // Update LRU Counter
        page_table[page].lru_counter = lru_counter++;
        if (lru_counter == MAX_LRU_COUNTER)
        {
            lru_counter = 0;
        }

        // Calculate Physical Address
        int phy_address = ((frame & 0x000000FF) << 8) + (offset & 0x000000FF);

        // Print Address and Value
        printf("\033[1;32m"); // Set color to green
        printf("+-----------------+-----------------+-------+\n");
        printf("| Virtual address | Physical address| Value |\n");
        printf("+-----------------+-----------------+-------+\n");
        printf("\033[0m"); // Reset color

        printf("| \033[1;31m%15d\033[0m | \033[1;33m%15d\033[0m | \033[1;34m%5d\033[0m |\n", log_address, phy_address, physical_memory[phy_address]); // Red, Yellow, Blue

        printf("\033[1;32m"); // Set color to green
        printf("+-----------------+-----------------+-------+\n");
        printf("\033[0m"); // Reset color
        num_memory_accesses++;
    }

    fclose(file);
}


void printStatistics()
{
    printf("\033[1;32m"); // Set color to green
    printf("+---------------------------------+---------+\n");
    printf("| Metric                          | Value   |\n");
    printf("+---------------------------------+---------+\n");
    printf("\033[0m"); // Reset color

    printf("| Page Table Size (# Pages)       | \033[1;31m%7d\033[0m |\n", PAGE_TABLE_SIZE); // Red
    printf("| Physical Memory Size (# Frames) | \033[1;33m%7d\033[0m |\n", PHYS_MEM_SIZE); // Yellow
    printf("| TLB Size (# Page-Frame Regs)    | \033[1;34m%7d\033[0m |\n", TLB_SIZE); // Blue
    printf("| Total Num Page Faults           | \033[1;35m%7d\033[0m |\n", num_page_faults); // Magenta
    printf("| Total TLB Hits                  | \033[1;36m%7d\033[0m |\n", num_tlb_hits); // Cyan
    printf("| Page Fault Rate                 | \033[1;37m%7.2f\033[0m |\n", (float)num_page_faults / (float)num_memory_accesses); // White
    printf("| TLB Hit Rate                    | \033[1;31m%7.2f\033[0m |\n", (float)num_tlb_hits / (float)num_memory_accesses); // Red
    printf("| Total Num Reads                 | \033[1;33m%7d\033[0m |\n", num_memory_reads); // Yellow
    printf("| Total Num Writes                | \033[1;34m%7d\033[0m |\n", num_memory_writes); // Blue
    printf("| Total Num Dirty Swaps           | \033[1;35m%7d\033[0m |\n", num_dirty_swaps); // Magenta

    printf("\033[1;32m"); // Set color to green
    printf("+---------------------------------+---------+\n");
    printf("\033[0m"); // Reset color
}