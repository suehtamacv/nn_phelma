#include "perceptron.h"

#pragma design
void percep4_apply(ac_channel<percep4_In_t> &I, ac_channel<percep4_Out_t> &Y)
{
    percep4_In_t  bufferI = I.read();
    percep4_Out_t bufferY;

    const perceptronKernel_t K[180 * 10] =
        {
#include "kernel_percep4.h"
        };
    const perceptronBias_t B[10] =
        {
#include "bias_percep4.h"
        };

loopY:
    for (unsigned int iKy = 0; iKy < 10; ++iKy)
        {
        bufferY.Y[iKy] = B[iKy];

loopX:
        for (unsigned int iKx = 0; iKx < 180; ++iKx)
            {
            bufferY.Y[iKy] += K[iKy * 180 + iKx] * bufferI.Y[iKx];
            }

#ifdef __STAT__
        std::cout << name << " Y " << Y[iKy] << std::endl;
#endif
        }

    Y.write(bufferY);

}
