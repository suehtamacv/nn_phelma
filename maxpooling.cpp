#include "maxpooling.h"

#pragma design
void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I,
                       ac_channel<maxPool1_line_Out_t> &Y)
{
#define sizeX 24
#define sizeY 24
#define sizeC 3
    maxPool1_line_In_t  bufferI_Old;
    maxPool1_line_In_t  bufferI_New = I.read();
    maxPool1_line_In_t  bufferI_Zero;
    maxPool1_line_Out_t bufferY;

    unsigned int nxI = 0;
    uint1 nyI = 0;

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
                layerOutBlock_t B[4];

                B[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];
                B[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];

                pixel_t Pix[4];

                Pix[0] = B[0].P[B[0].biggerBlock];
                Pix[1] = B[1].P[((uint2) B[1].biggerV) * 2];
                Pix[2] = B[2].P[B[0].biggerH];
                Pix[3] = B[3].P[0];

                pixel_t maxPixel = Pix[0];
loopFindMax:
                for (unsigned int i = 1; i < 4; ++i)
                    {
                    if (maxPixel < Pix[i])
                        {
                        maxPixel = Pix[i];
                        }
                    }

                bufferY.Y[cI * newSizeX + nxI].P[nyI * 2 + (nxI % 2)] = maxPixel;
                }
            }

        // Writes after each two lines
        if (nyI % 2 == 0 && yI != 0)
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

    unsigned int nxI = 0;
    uint1 nyI = 0;

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
                layerOutBlock_t B[4];

                B[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];
                B[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];

                pixel_t Pix[4];

                Pix[0] = B[0].P[B[0].biggerBlock];
                Pix[1] = B[1].P[((uint2) B[1].biggerV) * 2];
                Pix[2] = B[2].P[B[0].biggerH];
                Pix[3] = B[3].P[0];

                pixel_t maxPixel = Pix[0];
loopFindMax:
                for (unsigned int i = 1; i < 4; ++i)
                    {
                    if (maxPixel < Pix[i])
                        {
                        maxPixel = Pix[i];
                        }
                    }

                bufferY.Y[cI * newSizeX + nxI].P[nyI * 2 + (nxI % 2)] = maxPixel;
                }
            }

        // Writes after each two lines
        if (nyI % 2 == 0 && yI != 0)
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

    unsigned int nxI = 0;
    uint1 nyI = 0;

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
                layerOutBlock_t B[4];

                B[0] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[1] = bufferI_Old.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];
                B[2] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH)];
                B[3] = bufferI_New.Y[cI * (sizeX / BLOCK_WIDTH) + (xI / BLOCK_WIDTH) + 1];

                pixel_t Pix[4];

                Pix[0] = B[0].P[B[0].biggerBlock];
                Pix[1] = B[1].P[((uint2) B[1].biggerV) * 2];
                Pix[2] = B[2].P[B[0].biggerH];
                Pix[3] = B[3].P[0];

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
