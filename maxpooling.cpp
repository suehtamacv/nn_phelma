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
    maxPool1_line_In_t  bufferI_New = I.read();
    maxPool1_line_In_t  bufferI_Zero;
    maxPool1_line_Out_t bufferY;

    pixel_t Pix[4];

    unsigned int nxI = 1;
    unsigned int nyI = 1;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - stride)
            {
            bufferI_New = bufferI_Zero;
            }
        else
            {
            bufferI_New = I.read();
            }

loopInit:
        for (unsigned int i = 0; i < sizeC * (newSizeX + 2); ++i)
            {
            bufferY.Y[i][0] = 0;
            bufferY.Y[i][1] = 0;
            bufferY.Y[i][2] = 0;
            bufferY.Y[i][3] = 0;
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 1;
loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0];
                Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2];
                if (xI == sizeX - stride)
                    {
                    Pix[1] = 0;
                    Pix[3] = 0;
                    }
                else
                    {
                    Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1];
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

                bufferY.Y[cI * ((newSizeX + 2) / BLOCK_WIDTH) + nxI / BLOCK_WIDTH]
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
    maxPool2_line_In_t  bufferI_New = I.read();
    maxPool2_line_In_t  bufferI_Zero;
    maxPool2_line_Out_t bufferY;

    pixel_t Pix[4];

    unsigned int nxI = 1;
    unsigned int nyI = 1;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - stride)
            {
            bufferI_New = bufferI_Zero;
            }
        else
            {
            bufferI_New = I.read();
            }

loopInit:
        for (unsigned int i = 0; i < sizeC * (newSizeX + 2); ++i)
            {
            bufferY.Y[i][0] = 0;
            bufferY.Y[i][1] = 0;
            bufferY.Y[i][2] = 0;
            bufferY.Y[i][3] = 0;
            }

loopChannels:
        for (unsigned int cI = 0; cI < sizeC; ++cI)
            {
            nxI = 1;
loopX:
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {
                Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0];
                Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2];
                if (xI == sizeX - stride)
                    {
                    Pix[1] = 0;
                    Pix[3] = 0;
                    }
                else
                    {
                    Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1];
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

                bufferY.Y[cI * ((newSizeX + 2) / BLOCK_WIDTH) + nxI / BLOCK_WIDTH]
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
    maxPool3_line_In_t  bufferI_New = I.read();
    maxPool3_line_In_t  bufferI_Zero;
    maxPool3_line_Out_t bufferY;

    pixel_t Pix[4];

    layerOutBlock_t B_Zero;
    B_Zero[0] = 0;
    B_Zero[1] = 0;
    B_Zero[2] = 0;
    B_Zero[3] = 0;

    unsigned int nxI = 0;
    unsigned int nyI = 0;

loopY:
    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - stride)
            {
            bufferI_New = bufferI_Zero;
            }
        else
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
                Pix[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][0];
                Pix[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)][2];
                if (xI == sizeX - stride)
                    {
                    Pix[1] = 0;
                    Pix[3] = 0;
                    }
                else
                    {
                    Pix[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1][1];
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
