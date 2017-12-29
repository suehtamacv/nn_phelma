#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

#pragma design
template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void maxPooling_apply(ac_channel<lineBlockInterface<sizeX * sizeC> > &I,
                      ac_channel<lineBlockInterface<newSizeX * sizeC> > &Y)
{
    lineBlockInterface<sizeX * sizeC> bufferI_Old;
    lineBlockInterface<sizeX * sizeC> bufferI_New = I.read();
    lineBlockInterface<sizeX * sizeC> bufferI_Zero;
    lineBlockInterface<newSizeX * sizeC > bufferY;

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

                // Finds max and stores into Pix[0]
                if (Pix[0] < Pix[1])
                    {
                    Pix[0] = Pix[1];
                    }
                if (Pix[0] < Pix[2])
                    {
                    Pix[0] = Pix[2];
                    }
                if (Pix[0] < Pix[3])
                    {
                    Pix[0] = Pix[3];
                    }

                bufferY.Y[cI * newSizeX + nxI].P[nyI * 2 + (nxI % 2)] = Pix[0];
                }
            }

        // Writes after each two lines
        if (nyI % 2 == 0 && yI != 0)
            {
            Y.write(bufferY);
            }
        }

#undef T
}

#undef newSizeX
#undef newSizeY

#endif // MAXPOOLING_H
