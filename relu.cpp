#include "relu.h"

#define T(y, x)  I[(y) * sizeX * sizeC + (x) * sizeC + cI]

ReLU::ReLU()
{

}

uint8 ReLU::max(int8 x)
{
    if (x > 0)
        {
        return x;
        }
    else
        {
        return 0;
        }
}

uint8 *ReLU::apply(int8 *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC)
{
    uint8* Y = new uint8[sizeY * sizeX * sizeC]();

    for (unsigned int i = 0; i < sizeY * sizeX * sizeC; ++i)
        {
        Y[i] = max(I[i]);
        }

    return Y;
}

