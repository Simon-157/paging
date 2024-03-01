# Memory Management Simulator

## Overview

This project is a simulation of a memory management system in an operating system. It simulates processes accessing memory using a paging system. The paging system translates logical addresses to physical addresses. The project also includes a basic memory manager and a backing store.

## System Evaluation and Inferences

The performance of the memory management system can be evaluated by varying the process sizes and memory access patterns. Here are some inferences that can be drawn from such an evaluation:

- **Process Sizes**: As the size of the processes increases, the number of page faults may also increase. This is because larger processes are more likely to have pages that are not currently in memory, leading to more page faults. However, the impact of process size on the number of page faults can be mitigated by increasing the size of the physical memory or the page size.

- **Memory Access Patterns**: The pattern of memory accesses can significantly affect the performance of the memory management system. For example, if a process frequently accesses a small set of pages, the number of page faults will be low because those pages will likely be kept in memory. On the other hand, if a process randomly accesses a large set of pages, the number of page faults will be high because the required pages are less likely to be in memory.



## Backing Store

The `backing_store.bin` file is a binary file that acts as the backing store for the memory management system. It is used to store the pages that are not currently in memory. When a page fault occurs, the required page is loaded from the backing store into memory.

The backing store is managed by the functions defined in `backing_store.h`:

- `writeOutFrameToStore(int phy_frame)`: Writes a frame from memory to the backing store.
- `readInFrameFromStore(int phy_frame, int store_frame)`: Reads a frame from the backing store into memory.
- `loadPageFromBackingStore(int page, char *frame_start, int process_id)`: Loads a page from the backing store into a frame in memory.

The `backing_store.bin` file is created and managed by the program, and its contents change as the program runs. It is an essential part of the simulation of the memory management system, as it allows the system to handle more processes than can fit into physical memory at once.
## Features

- **Multiple Process Simulation**: The program simulates multiple processes accessing memory. Each process has its own page table.
- **Address Translation**: The program translates logical addresses to physical addresses using a paging system.
- **Page Fault Handling**: The program handles page faults by loading pages from a backing store.
- **Memory Management**: The program includes a basic memory manager that manages the allocation and deallocation of memory.
- **Statistics**: The program prints statistics about the memory accesses, including the number of page faults and TLB hits.
## Parameters

The project uses several parameters defined in `params.h` to control the behavior of the memory management system:

- `PAGE_SIZE` and `FRAME_SIZE`: These define the size of a page and a frame, respectively. Both are set to 4096 bytes (4KB).
- `PAGE_TABLE_SIZE`: This is the number of pages allowed in the page table. It's calculated as the physical memory size divided by the page size.
- `PHYS_MEM_SIZE`: This is the size of the physical memory, set to 1048576 bytes (1MB).
- `TLB_SIZE`: This is the number of entries in the Translation Lookaside Buffer (TLB), set to 32 entries.
- `VIRTUAL_PAGE_MASK`, `VIRTUAL_OFFSET_MASK`, `VIRTUAL_PAGE_SHIFT`, `VIRTUAL_OFFSET_SHIFT`: These are used for extracting the page number and offset from a virtual address.
- `PAGE_ENTRY_VALID_MASK`, `PAGE_DIRTY_BIT_MASK`, `PAGE_LRU_COUNTER_MASK`, `PAGE_FRAME_NUM_MASK`, `PAGE_ENTRY_VALID_SHIFT`, `PAGE_DIRTY_BIT_SHIFT`, `PAGE_LRU_COUNTER_SHIFT`, `PAGE_FRAME_NUM_SHIFT`: These are used for managing the page table entries.
- `TLB_ENTRY_VALID_MASK`, `TLB_DIRTY_BIT_MASK`, `TLB_PAGE_NUM_MASK`, `TLB_FRAME_NUM_MASK`, `TLB_ENTRY_VALID_SHIFT`, `TLB_DIRTY_BIT_SHIFT`, `TLB_PAGE_NUM_SHIFT`, `TLB_FRAME_NUM_SHIFT`: These are used for managing the TLB entries.Management
- `MAX_LRU_COUNTER`, `PT_LRU_COUNTER_SHIFT`, `PT_LRU_COUNTER_MASK`: These are used for implementing the Least Recently Used (LRU) page replacement algorithm.

These parameters can be adjusted to change the behavior of the memory management system.


## Project Structure

The project is organized into several directories:

- `src`: This directory contains the source code of the project. It has two subdirectories:
    - `lib`: Contains the source code files (`*.c`).
    - `include`: Contains the header files (`*.h`).
- `data`: This directory contains the data files used by the project:
    - `backing_store.bin`: The binary file that acts as the backing store.
    - `random_addresses.txt`: A file containing random addresses used for testing the memory management system.
- `tests`: This directory contains unit tests for the project.

Here is a visual representation of the project structure:

## Code Structure

- `main.c`: Contains the main function which controls the simulation.
- `memory_manager.h`: Contains the definition of the memory manager. It includes functions for getting frames from the page table and TLB, updating the TLB, accessing memory, and loading pages into memory.
- `virtual_memory.h`: Contains the definition of the virtual memory. It includes functions for translating virtual addresses to physical addresses.
- `backing_store.h`: Contains the definition of the backing store. It includes functions for reading and writing to the backing store.
- `process.h`: Contains the definition of a process. It includes functions for creating and managing processes.

## Installation and Usage

1. Clone the repository: `git clone https://github.com/Simon-157/paging.git`
2. Navigate to the project directory: `cd paging`
3. Compile the project using the provided Makefile: `make`
4. Run the program with the number of processes, number of memory accesses, and process size as command line arguments: `./paging <num_processes> <num_memory_accesses> <process_size>`

## Contributing

We welcome contributions to this project. Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your changes.
3. Commit your changes to your branch.
4. Push your changes and create a pull request.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.