#include "perceptron.h"
#include "kernels.h"
#include "biases.h"

#pragma design
void percep4_apply(ac_channel<percep4_In_t> &I, ac_channel<percep4_Out_t> &Y)
{
    percep4_In_t  bufferI = I.read();
    percep4_Out_t bufferY;

loopY:
    for (unsigned int iKy = 0; iKy < 10; ++iKy)
        {
        bufferY.Y[iKy] = perceptronBias4[iKy];

loopX:
        for (unsigned int iKx = 0; iKx < 180; ++iKx)
            {
            bufferY.Y[iKy] += perceptronKernel4[iKy * 180 + iKx] * bufferI.Y[iKx];
            }
        }

    Y.write(bufferY);

}
