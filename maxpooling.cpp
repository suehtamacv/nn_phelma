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

    maxPool1_line_In_t  bufferI_Old;
    maxPool1_line_In_t  bufferI_New;
    maxPool1_line_Out_t bufferY;

    pixel_t Pix[4];

    unsigned int nxI = 0;
    unsigned int nyI = 0;

loopY:
    for (unsigned int yI = 0; yI < sizeY + BLOCK_HEIGHT; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY + BLOCK_HEIGHT - stride;
        bufferI_Old = bufferI_New;
        if (!yLimit)
            {
            bufferI_New = I.read();
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
                        Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1][0];

                        if (xLimit)
                            {
                            Pix[1] = 0;
                            }
                        else
                            {
                            Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][1];
                            }

                        if (yLimit)
                            {
                            Pix[2] = 0;
                            }
                        else
                            {
                            Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1][2];
                            }

                        if (xLimit || yLimit)
                            {
                            Pix[3] = 0;
                            }
                        else
                            {
                            Pix[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][3];
                            }

loopFindMax:
                        for (unsigned int i = 0; i < 4; ++i)
                            {
                            if (maxPixel < Pix[i])
                                {
                                maxPixel = Pix[i];
                                }
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

    maxPool2_line_In_t  bufferI_Old;
    maxPool2_line_In_t  bufferI_New;
    maxPool2_line_Out_t bufferY;

    pixel_t Pix[4];

    unsigned int nxI = 0;
    unsigned int nyI = 0;

loopY:
    for (unsigned int yI = 0; yI < sizeY + BLOCK_HEIGHT; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY + BLOCK_HEIGHT - stride;
        bufferI_Old = bufferI_New;
        if (!yLimit)
            {
            bufferI_New = I.read();
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
                        Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1][0];

                        if (xLimit)
                            {
                            Pix[1] = 0;
                            }
                        else
                            {
                            Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][1];
                            }

                        if (yLimit)
                            {
                            Pix[2] = 0;
                            }
                        else
                            {
                            Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) - 1][2];
                            }

                        if (xLimit || yLimit)
                            {
                            Pix[3] = 0;
                            }
                        else
                            {
                            Pix[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][3];
                            }

loopFindMax:
                        for (unsigned int i = 0; i < 4; ++i)
                            {
                            if (maxPixel < Pix[i])
                                {
                                maxPixel = Pix[i];
                                }
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

    maxPool3_line_In_t  bufferI_Old;
    maxPool3_line_In_t  bufferI_New;
    maxPool3_line_Out_t bufferY;

    pixel_t Pix[4];

    unsigned int nxI = 0;
    unsigned int nyI = 0;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bool yLimit = yI == sizeY + BLOCK_HEIGHT - stride;
        bufferI_Old = bufferI_New;
        if (!yLimit)
            {
            bufferI_New = I.read();
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 0;
loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                bool xLimit = xI == sizeX + BLOCK_WIDTH - stride;

                Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0];

                if (xLimit)
                    {
                    Pix[1] = 0;
                    }
                else
                    {
                    Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1];
                    }

                if (yLimit)
                    {
                    Pix[2] = 0;
                    }
                else
                    {
                    Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2];
                    }

                if (xLimit)
                    {
                    Pix[3] = 0;
                    }
                else
                    {
                    Pix[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][3];
                    }

                pixel_t maxPixel = Pix[0];
loopFindMax:
                for (unsigned int i = 1; i < 4; ++i)
                    {
                    if (maxPixel < Pix[i])
                        {
                        maxPixel = Pix[i];
                        }
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
