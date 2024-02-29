#include "virtual_memory.h"

int getPageNumber(int address) {
    return (address & VIRTUAL_PAGE_MASK) >> VIRTUAL_PAGE_SHIFT;
}

int getOffset(int address) {
    return (address & VIRTUAL_OFFSET_MASK) >> VIRTUAL_OFFSET_SHIFT;
}
