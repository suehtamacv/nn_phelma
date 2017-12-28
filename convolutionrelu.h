#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define overlap  2
#define tileSize 4

void calculateG(convKernel_t (&G)[16], const convKernel_t *K);
void calculateD(pixel_t (&Block)[16], convD_t (&D)[16]);

void conv1_apply(ac_channel<conv1_line_In_t> &I, ac_channel<conv1_line_Out_t> &Y);
void conv2_apply(ac_channel<conv2_In_t> &I, ac_channel<conv2_Out_t> &Y);
void conv3_apply(ac_channel<conv3_In_t> &I, ac_channel<conv3_Out_t> &Y);

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void getImageBlock(lineBlockInterface<sizeX * sizeC> &I, pixel_t (&Block)[16], const unsigned int xI,
                   const unsigned int yI, const unsigned int cI)
{
#define B(y, x) \
    Block[y * tileSize + x]

#define T(y, x) \
    I.Y[cI * (sizeY / 2) * (sizeX / 2) + (yI + (y)) * (sizeX / 2) + (xI + (x))]

loopImageBlockY:
    for (unsigned int j = 0; j < 1; ++j)
        {
loopImageBlockX:
        for (unsigned int i = 0; i < 1; ++i)
            {
            unsigned int index = 8 * j + 2 * i;
            layerOut_t rawBlock = T(j, i);

            Block[index + 0].set_slc(0, rawBlock.slc<12>(0));
            Block[index + 1].set_slc(0, rawBlock.slc<12>(12));
            Block[index + 4].set_slc(0, rawBlock.slc<12>(24));
            Block[index + 5].set_slc(0, rawBlock.slc<12>(36));
            }
        }

#undef T
#undef B
}

#endif // CONVOLUTION_H
