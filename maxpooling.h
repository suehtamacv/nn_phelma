#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define stride    2
#define poolSize  3
#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

template<unsigned int sizeX, typename T>
pixel_t getMaxPixel(T& Old, T& New, unsigned int xI, unsigned int cI, bool xLim, bool yLim)
{
    pixel_t maxPixel = 0;

    if (maxPixel < Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0])
        {
        maxPixel = Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0];
        }
    if (!xLim)
        {
        if (maxPixel < Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1])
            {
            maxPixel = Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1];
            }
        }
    if (!yLim)
        {
        if (maxPixel < New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2])
            {
            maxPixel = New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2];
            }
        }
    if (!xLim && !yLim)
        {
        if (maxPixel < New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][3])
            {
            maxPixel = New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][3];
            }
        }

    return maxPixel;
}

void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I, ac_channel<maxPool1_line_Out_t> &Y);
void maxPooling2_apply(ac_channel<maxPool2_line_In_t> &I, ac_channel<maxPool2_line_Out_t> &Y);
void maxPooling3_apply(ac_channel<maxPool3_line_In_t> &I, ac_channel<maxPool3_line_Out_t> &Y);

#endif // MAXPOOLING_H
