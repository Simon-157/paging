#include <stdlib.h>
#include "process.h"
#include <params.h>

ProcessQueue* createProcessQueue(int capacity) {
    ProcessQueue* queue = (ProcessQueue*) malloc(sizeof(ProcessQueue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->processes = (Process*) malloc(capacity * sizeof(Process));
    return queue;
}

void enqueueProcess(ProcessQueue* queue, Process process) {
    if (queue->size == queue->capacity) {
        return; // Queue is full
    }
    queue->processes[queue->size++] = process;
}



Process dequeueProcess(ProcessQueue* queue) {
    if (queue->size == 0) {
        Process emptyProcess = {0, 0, NULL};
        return emptyProcess; // Queue is empty
    }
    Process process = queue->processes[0];
    for (int i = 0; i < queue->size - 1; i++) {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->size--;
    return process;
}

void freeProcessQueue(ProcessQueue* queue) {
    free(queue->processes);
    free(queue);
}