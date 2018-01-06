#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define stride    2
#define poolSize  3
#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

template<unsigned int sizeX, unsigned int sizeY, typename T>
pixel_t getMaxPixel(T& Buf, unsigned int xI, unsigned int yI, unsigned int cI, bool xLim, bool yLim)
{
    pixel_t maxPixel = 0;

    unsigned int index = cI * (sizeY / BLOCK_HEIGHT) * (sizeX / BLOCK_WIDTH) +
                         (yI / BLOCK_HEIGHT) * (sizeX / BLOCK_WIDTH) +
                         (xI / BLOCK_WIDTH);

    if (maxPixel < Buf.Y[index][0])
        {
        maxPixel = Buf.Y[index][0];
        }
    if (!xLim)
        {
        if (maxPixel < Buf.Y[index + 1][1])
            {
            maxPixel = Buf.Y[index + 1][1];
            }
        }
    if (!yLim)
        {
        if (maxPixel < Buf.Y[index + (sizeX / BLOCK_WIDTH)][2])
            {
            maxPixel = Buf.Y[index + (sizeX / BLOCK_WIDTH)][2];
            }
        }
    if (!xLim && !yLim)
        {
        if (maxPixel < Buf.Y[index + (sizeX / BLOCK_WIDTH) + 1][3])
            {
            maxPixel = Buf.Y[index + (sizeX / BLOCK_WIDTH) + 1][3];
            }
        }

    return maxPixel;
}

void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I, ac_channel<maxPool1_line_Out_t> &Y);
void maxPooling2_apply(ac_channel<maxPool2_line_In_t> &I, ac_channel<maxPool2_line_Out_t> &Y);
void maxPooling3_apply(ac_channel<maxPool3_line_In_t> &I, ac_channel<maxPool3_line_Out_t> &Y);

#endif // MAXPOOLING_H
