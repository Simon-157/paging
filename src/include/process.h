// #ifndef PROCESS_H
// #define PROCESS_H

// typedef struct Process {
//     int id;
//     int size;
//     int *memory_addresses;
// } Process;

// typedef struct ProcessQueue {
//     Process *processes;
//     int size;
//     int capacity;
// } ProcessQueue;

// ProcessQueue* createProcessQueue(int capacity);
// void enqueueProcess(ProcessQueue* queue, Process process);
// Process dequeueProcess(ProcessQueue* queue);
// void freeProcessQueue(ProcessQueue* queue);

// #endif // PROCESS_H




#ifndef PROCESS_H
#define PROCESS_H

typedef struct PageTableEntry {
    int valid;
    int frame_number;
    int dirty;
} PageTableEntry;

typedef struct Process {
    int id;
    int size;
    int *memory_addresses;
    PageTableEntry **page_table; // Add this line
} Process;

typedef struct ProcessQueue {
    Process *processes;
    int size;
    int capacity;
} ProcessQueue;

ProcessQueue* createProcessQueue(int capacity);
void enqueueProcess(ProcessQueue* queue, Process process);
Process dequeueProcess(ProcessQueue* queue);
void freeProcessQueue(ProcessQueue* queue);

#endif // PROCESS_H