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

    maxPool1_line_In_t  bufferI_1 = I.read();
    maxPool1_line_In_t  bufferI_2;
    maxPool1_line_Out_t bufferY;

    unsigned int nxI = 1;
    unsigned int nyI = 1;

    bool toggleMemory = true;

loopFirstLine:
    for (unsigned int i = 0; i < sizeC * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][0] = 0;
        bufferY.Y[i][1] = 0;
        }

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

        toggleMemory = !toggleMemory;
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
            nxI = 1;

            //First pixels of line
            bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1)][0] = 0;
            bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1)][2] = 0;

            //Last pixels of line
            bufferY.Y[(cI + 1) * ((newSizeX / BLOCK_WIDTH) + 1) - 1][1] = 0;
            bufferY.Y[(cI + 1) * ((newSizeX / BLOCK_WIDTH) + 1) - 1][3] = 0;

loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                if (!toggleMemory)
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool1_line_In_t>(bufferI_1, bufferI_2, xI, cI, xLimit, yLimit);
                    }
                else
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool1_line_In_t>(bufferI_2, bufferI_1, xI, cI, xLimit, yLimit);
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

loopLastLine:
    for (unsigned int i = 0; i < sizeC * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][2] = 0;
        bufferY.Y[i][3] = 0;
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

    maxPool2_line_In_t  bufferI_1 = I.read();
    maxPool2_line_In_t  bufferI_2;
    maxPool2_line_Out_t bufferY;

    unsigned int nxI = 1;
    unsigned int nyI = 1;

    bool toggleMemory = true;

loopFirstLine:
    for (unsigned int i = 0; i < sizeC * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][0] = 0;
        bufferY.Y[i][1] = 0;
        }

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

        toggleMemory = !toggleMemory;
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
            nxI = 1;

            //First pixels of line
            bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1)][0] = 0;
            bufferY.Y[cI * ((newSizeX / BLOCK_WIDTH) + 1)][2] = 0;

            //Last pixels of line
            bufferY.Y[(cI + 1) * ((newSizeX / BLOCK_WIDTH) + 1) - 1][1] = 0;
            bufferY.Y[(cI + 1) * ((newSizeX / BLOCK_WIDTH) + 1) - 1][3] = 0;

loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                if (!toggleMemory)
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool2_line_In_t>(bufferI_1, bufferI_2, xI, cI, xLimit, yLimit);
                    }
                else
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool2_line_In_t>(bufferI_2, bufferI_1, xI, cI, xLimit, yLimit);
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

loopLastLine:
    for (unsigned int i = 0; i < sizeC * ((newSizeX / BLOCK_WIDTH) + 1); ++i)
        {
        bufferY.Y[i][2] = 0;
        bufferY.Y[i][3] = 0;
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

    maxPool3_line_In_t  bufferI_1 = I.read();
    maxPool3_line_In_t  bufferI_2;
    maxPool3_line_Out_t bufferY;

    unsigned int nxI = 0;
    unsigned int nyI = 0;

    bool toggleMemory = true;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY - stride;

        toggleMemory = !toggleMemory;
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
                pixel_t maxPixel = 0;
                bool xLimit = xI == sizeX - stride;

                if (!toggleMemory)
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool3_line_In_t>(bufferI_1, bufferI_2, xI, cI, xLimit, yLimit);
                    }
                else
                    {
                    maxPixel = getMaxPixel<sizeX, maxPool3_line_In_t>(bufferI_2, bufferI_1, xI, cI, xLimit, yLimit);
                    }

                bufferY.Y[cI * newSizeY * newSizeX + nyI * newSizeX + nxI] = maxPixel;
                }
            }
        }

    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
}

