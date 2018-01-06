#include "maxpooling.h"
#include "fixedpointvariables.h"
#include "meminterface.h"

#pragma design
void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I,
                       ac_channel<maxPool1_line_Out_t> &Y)
{
#define sizeX 24
#define sizeY 24
#define sizeC 64

    maxPool1_line_In_t  bufferI = I.read();
    maxPool1_line_Out_t bufferY;

    unsigned int nxI = 1;
    unsigned int nyI = 1;

loopInit:
    for (unsigned int i = 0; i < sizeC * ((newSizeY / BLOCK_HEIGHT) + 1) * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][0] = 0;
        bufferY.Y[i][1] = 0;
        bufferY.Y[i][2] = 0;
        bufferY.Y[i][3] = 0;
        }

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 1;

loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                maxPixel = getMaxPixel<sizeX, sizeY>(bufferI, xI, yI, cI, xLimit, yLimit);

                unsigned int index = cI * ((newSizeY / BLOCK_HEIGHT) + 1) * ((newSizeX / BLOCK_WIDTH) + 1)
                                     + (nyI / BLOCK_HEIGHT) * ((newSizeX / BLOCK_WIDTH) + 1)
                                     + nxI / BLOCK_WIDTH;
                bufferY.Y[index][(nyI % 2) * 2 + (nxI % 2)] = maxPixel;
                }
            }
        }

    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
}

#pragma design
void maxPooling2_apply(ac_channel<maxPool2_line_In_t> &I,
                       ac_channel<maxPool2_line_Out_t> &Y)
{
#define sizeX 12
#define sizeY 12
#define sizeC 32

    maxPool2_line_In_t  bufferI = I.read();
    maxPool2_line_Out_t bufferY;

    unsigned int nxI = 1;
    unsigned int nyI = 1;

loopInit:
    for (unsigned int i = 0; i < sizeC * ((newSizeY / BLOCK_HEIGHT) + 1) * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][0] = 0;
        bufferY.Y[i][1] = 0;
        bufferY.Y[i][2] = 0;
        bufferY.Y[i][3] = 0;
        }

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 1;

loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                maxPixel = getMaxPixel<sizeX, sizeY>(bufferI, xI, yI, cI, xLimit, yLimit);

                unsigned int index = cI * ((newSizeY / BLOCK_HEIGHT) + 1) * ((newSizeX / BLOCK_WIDTH) + 1)
                                     + (nyI / BLOCK_HEIGHT) * ((newSizeX / BLOCK_WIDTH) + 1)
                                     + nxI / BLOCK_WIDTH;
                bufferY.Y[index][(nyI % 2) * 2 + (nxI % 2)] = maxPixel;
                }
            }
        }

    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
}

#pragma design
void maxPooling3_apply(ac_channel<maxPool3_line_In_t> &I,
                       ac_channel<maxPool3_line_Out_t> &Y)
{
#define sizeX 6
#define sizeY 6
#define sizeC 20

    maxPool3_line_In_t  bufferI = I.read();
    maxPool3_line_Out_t bufferY;

    unsigned int nxI = 1;
    unsigned int nyI = 1;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 1;

loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                maxPixel = getMaxPixel<sizeX, sizeY>(bufferI, xI, yI, cI, xLimit, yLimit);

                bufferY.Y[cI * newSizeY * newSizeX + nyI * newSizeX + nxI] = maxPixel;
                }
            }
        }

    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
}

