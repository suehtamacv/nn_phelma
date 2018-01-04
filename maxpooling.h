#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define stride    2
#define poolSize  3
#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

pixel_t getMaxPixel(layerOutBlock_t& B0, layerOutBlock_t& B1, layerOutBlock_t& B2, layerOutBlock_t& B3,
                    bool xLim, bool yLim);

void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I, ac_channel<maxPool1_line_Out_t> &Y);
void maxPooling2_apply(ac_channel<maxPool2_line_In_t> &I, ac_channel<maxPool2_line_Out_t> &Y);
void maxPooling3_apply(ac_channel<maxPool3_line_In_t> &I, ac_channel<maxPool3_line_Out_t> &Y);

#endif // MAXPOOLING_H
