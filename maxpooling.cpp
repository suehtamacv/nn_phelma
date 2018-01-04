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

    maxPool1_line_In_t  bufferI_1;
    maxPool1_line_In_t  bufferI_2;
    maxPool1_line_Out_t bufferY;

    unsigned int nxI = 0;
    unsigned int nyI = 0;

    bool toggleMemory = true;

loopY:
    for (unsigned int yI = 0; yI < sizeY + BLOCK_HEIGHT; yI += stride, ++nyI)
        {
        toggleMemory = !toggleMemory;
        bool yLimit = yI == sizeY + BLOCK_HEIGHT - stride;
        if (!yLimit)
            {
            if (toggleMemory)
                {
                bufferI_1 = I.read();
                }
            else
                {
                bufferI_2 = I.read();
                }
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 0;
loopX:
            for (unsigned int xI = 0; xI < sizeX + BLOCK_WIDTH; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;

                if (nyI != 0 && nyI != newSizeY + 1)
                    {
                    bool xLimit = xI == sizeX + BLOCK_WIDTH - stride;

                    if (nxI != 0 && nxI != newSizeX + 1)
                        {
                        if (!toggleMemory)
                            {
                            maxPixel = getMaxPixel(bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   xLimit, yLimit);
                            }
                        else
                            {
                            maxPixel = getMaxPixel(bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   xLimit, yLimit);
                            }
                        }
                    }

                bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1) + nxI / BLOCK_WIDTH]
                [(nyI % 2) * 2 + (nxI % 2)] = maxPixel;
                }
            }

        // Writes after each two lines
        if (nyI % 2 == 1)
            {
            Y.write(bufferY);
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

    maxPool2_line_In_t  bufferI_1;
    maxPool2_line_In_t  bufferI_2;
    maxPool2_line_Out_t bufferY;

    unsigned int nxI = 0;
    unsigned int nyI = 0;

    bool toggleMemory = true;

loopY:
    for (unsigned int yI = 0; yI < sizeY + BLOCK_HEIGHT; yI += stride, ++nyI)
        {
        toggleMemory = !toggleMemory;
        bool yLimit = yI == sizeY + BLOCK_HEIGHT - stride;
        if (!yLimit)
            {
            if (toggleMemory)
                {
                bufferI_1 = I.read();
                }
            else
                {
                bufferI_2 = I.read();
                }
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 0;

loopX:
            for (unsigned int xI = 0; xI < sizeX + BLOCK_WIDTH; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;

                if (nyI != 0 && nyI != newSizeY + 1)
                    {
                    bool xLimit = xI == sizeX + BLOCK_WIDTH  - stride;

                    if (nxI != 0 && nxI != newSizeX + 1)
                        {
                        if (!toggleMemory)
                            {
                            maxPixel = getMaxPixel(bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   xLimit, yLimit);
                            }
                        else
                            {
                            maxPixel = getMaxPixel(bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1],
                                                   bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                                   xLimit, yLimit);
                            }
                        }
                    }

                bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1) + nxI / BLOCK_WIDTH]
                [(nyI % 2) * 2 + (nxI % 2)] = maxPixel;
                }
            }

        // Writes after each two lines
        if (nyI % 2 == 1)
            {
            Y.write(bufferY);
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

    maxPool3_line_In_t  bufferI_1;
    maxPool3_line_In_t  bufferI_2 = I.read();
    maxPool3_line_Out_t bufferY;

    unsigned int nxI = 0;
    unsigned int nyI = 0;

    bool toggleMemory = false;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        toggleMemory = !toggleMemory;
        bool yLimit = yI == sizeY - stride;
        if (!yLimit)
            {
            if (toggleMemory)
                {
                bufferI_1 = I.read();
                }
            else
                {
                bufferI_2 = I.read();
                }
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 0;
loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                bool xLimit = xI == sizeX - stride;

                pixel_t maxPixel;

                if (!toggleMemory)
                    {
                    maxPixel = getMaxPixel(bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                           bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1],
                                           bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                           bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1],
                                           xLimit, yLimit);
                    }
                else
                    {
                    maxPixel = getMaxPixel(bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                           bufferI_2.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1],
                                           bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)],
                                           bufferI_1.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1],
                                           xLimit, yLimit);
                    }

                bufferY.Y[cI * newSizeX * newSizeY + nyI * newSizeX + nxI] = maxPixel;
                }
            }
        }
    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
}

pixel_t getMaxPixel(layerOutBlock_t &B0, layerOutBlock_t &B1, layerOutBlock_t &B2, layerOutBlock_t &B3,
                    bool xLim, bool yLim)
{
    pixel_t maxPixel = 0;

    if (maxPixel < B0[0])
        {
        maxPixel = B0[0];
        }
    if (!xLim && maxPixel < B1[1])
        {
        maxPixel = B1[1];
        }
    if (!yLim && maxPixel < B2[2])
        {
        maxPixel = B2[2];
        }
    if (!xLim & !yLim && maxPixel < B3[3])
        {
        maxPixel = B3[3];
        }

    return maxPixel;
}
