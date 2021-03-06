#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define overlap  2
#define tileSize 4

void calculateG(convKernel_t (&G)[16], const convKernel_t *K);
void calculateD(pixel_t (&Block)[16], convD_t (&D)[16]);

void conv1_apply(ac_channel<conv1_line_In_t> &I, ac_channel<conv1_line_Out_t> &Y);
void conv2_apply(ac_channel<conv2_line_In_t> &I, ac_channel<conv2_line_Out_t> &Y);
void conv3_apply(ac_channel<conv3_line_In_t> &I, ac_channel<conv3_line_Out_t> &Y);

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void getImageBlock(memBlockInterface<sizeX * sizeY * sizeC> &Buf, pixel_t (&Block)[16],
                   const unsigned int xI, const unsigned yI, const unsigned int cI)
{
    unsigned int bckIndex = cI * (sizeY / BLOCK_HEIGHT) * (sizeX / BLOCK_WIDTH) +
                            (yI / BLOCK_HEIGHT) * (sizeX / BLOCK_WIDTH) +
                            (xI / BLOCK_WIDTH);

loopImageBlockX:
    for (unsigned int i = 0; i < 2; ++i)
        {
        Block[2 * i + 0] = Buf.Y[bckIndex + i][0];
        Block[2 * i + 1] = Buf.Y[bckIndex + i][1];
        Block[2 * i + 4] = Buf.Y[bckIndex + i][2];
        Block[2 * i + 5] = Buf.Y[bckIndex + i][3];

        Block[2 * i + 8] = Buf.Y[bckIndex + (sizeX / BLOCK_WIDTH) + i][0];
        Block[2 * i + 9] = Buf.Y[bckIndex + (sizeX / BLOCK_WIDTH) + i][1];
        Block[2 * i + 12] = Buf.Y[bckIndex + (sizeX / BLOCK_WIDTH) + i][2];
        Block[2 * i + 13] = Buf.Y[bckIndex + (sizeX / BLOCK_WIDTH) + i][3];
        }
}
#endif // CONVOLUTION_H
