#ifndef BACKING_STORE_PROCESSING_H
#define BACKING_STORE_PROCESSING_H
#include "memory_manager.h"

void writeOutFrameToStore(int phy_frame);
void readInFrameFromStore(int phy_frame, int store_frame);
void loadPageFromBackingStore(int page, char *frame_start, int process_id);

#endif /* BACKING_STORE_PROCESSING_H */
