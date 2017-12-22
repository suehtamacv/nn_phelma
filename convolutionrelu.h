#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define overlap  2
#define tileSize 4

void calculateG(convKernel_t (&G)[16], const convKernel_t *K);
void calculateD(layerOut_t (&Block)[16], convD_t (&D)[16]);

void conv1_apply(ac_channel<conv1_In_t> &I, ac_channel<conv1_Out_t> &Y);
void conv2_apply(ac_channel<conv2_In_t> &I, ac_channel<conv2_Out_t> &Y);
void conv3_apply(ac_channel<conv3_In_t> &I, ac_channel<conv3_Out_t> &Y);

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void getImageBlock(memInterface<sizeY * sizeX * sizeC> &I, layerOut_t *Block, const unsigned int xI,
                   const unsigned int yI, const unsigned int cI)
{
#define B(x, y) \
    Block[y * tileSize + x]

#ifdef __HWC__
#define T(y, x) \
    I.Y[(yI + (y)) * sizeX * sizeC + (xI + (x)) * sizeC + cI]
#else
#define T(y, x) \
    I.Y[cI * sizeY * sizeX + (yI + (y)) * sizeX + (xI + (x))]
#endif


    const bool xBorderL = (xI == 0);
    const bool xBorderR = (xI + tileSize > sizeX);
    const bool yBorderT = (yI == 0);
    const bool yBorderB = (yI + tileSize > sizeY);

    const unsigned int yLimInf = yBorderT ? 1 : 0;
    const unsigned int yLimSup = yBorderB ? 2 : 3;
    const unsigned int xLimInf = xBorderL ? 1 : 0;
    const unsigned int xLimSup = xBorderR ? 2 : 3;

loopImageBlockY:
    for (unsigned int j = 0; j < 4; ++j)
        {
loopImageBlockX:
        for (unsigned int i = 0; i < 4; ++i)
            {
            B(j, i) = 0;
            if ((j >= yLimInf) && (j <= yLimSup) && (i >= xLimInf) && (i <= xLimSup))
                {
                B(j, i) = T(j - 1, i - 1);
                }
            }
        }

#undef T
#undef B
}


#endif // CONVOLUTION_H
