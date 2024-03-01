#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "params.h"
#include "statistics.h"
#include "memory_manager.h"     
#include "virtual_memory.h"
#include "backing_store.h"
#include "process.h"

#define RANDOM_ADDRESSES "data/random_addresses.txt"


void simulateMemoryAccess(Process process, int num_memory_accesses);
void printStatistics();

int main(int argc, char *argv[])
{
    initializePageTable();
    srand(time(NULL));

    if (argc != 4) {
        printf("Usage: %s <num_processes> <num_memory_accesses> <process_size>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    int num_memory_accesses = atoi(argv[2]);
    int process_size = atoi(argv[3]);

    // Create a queue of processes
    ProcessQueue* queue = createProcessQueue(num_processes);

    // Create processes and allocate them to memory pages
    for (int i = 0; i < num_processes; ++i)
    {
        Process process = {i, process_size, NULL};
        enqueueProcess(queue, process);
    }

    // Simulate memory access for each process
    while (queue->size > 0) {
        Process process = dequeueProcess(queue);
        simulateMemoryAccess(process, num_memory_accesses);
    }

    // Free the process queue
    freeProcessQueue(queue);

    // Print Statistics
    printStatistics();

    return 0;
}

void simulateMemoryAccess(Process process, int num_memory_accesses)
{
    FILE *file = fopen(RANDOM_ADDRESSES, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
     int count = 0;
    while (fgets(line, sizeof(line), file) && count < num_memory_accesses)
    {

        char *ptr;
        int log_address = strtol(line, &ptr, 10);
        char type = *ptr;

        int page = getPageNumber(log_address);
        int offset = getOffset(log_address);

        // Check TLB
        int frame = getFrameFromTLB(page, type == 'W'); 

        if (frame == -1) {
            // Page fault, check page table
            frame = getFrameFromPageTable(page, type == 'W'); 
            if (frame == -1) {
                // Page not in memory, load from backing store
                frame = loadPageIntoMemory(page, process.id);
            }
            // Add page to TLB
            addToTLB(page, frame);
        }

        // Calculate physical address and access memory
        int physical_address = translateVirtualToPhysical(log_address, frame);
        char value = accessMemory(physical_address, type == 'W');

        // Print Address and Value
        printf("\033[1;32m"); // Set color to green
        printf("+-----------------+-----------------+-------+\n");
        printf("| Logical address | Physical address| Value |\n");
        printf("+-----------------+-----------------+-------+\n");
        printf("\033[0m"); // Reset color

        printf("| \033[1;31m%15d\033[0m | \033[1;33m%15d\033[0m | \033[1;34m%5d\033[0m |\n", log_address, physical_address, physical_memory[physical_address].page_contents); // Red, Yellow, Blue

        printf("\033[1;32m"); // Set color to green
        printf("+-----------------+-----------------+-------+\n");
        printf("\033[0m"); // Reset color
        count++;
    }

    fclose(file);
}


