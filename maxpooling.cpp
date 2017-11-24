#include "maxpooling.h"

MaxPooling::MaxPooling(unsigned int stride) : stride(stride)
{

}

uint8* MaxPooling::apply(uint8 *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC)
{
    const unsigned int newSizeX = sizeX / stride;
    const unsigned int newSizeY = sizeY / stride;

    unsigned int nxI = 0, nyI = 0;

    uint8* Y = new uint8[newSizeY * newSizeX * sizeC];

    for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
        {
        nxI = 0;
        for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
            {

            for (unsigned int cI = 0; cI < sizeC; ++cI)
                {
                const unsigned int offsetY = nyI * newSizeX * sizeC + nxI * sizeC + cI;
                const unsigned int offsetI = yI * sizeX * sizeC + xI * sizeC + cI;
                Y[offsetY] = I[offsetI];

                for (unsigned int yO = 0; yO < stride; ++yO)
                    {
                    for (unsigned int xO = 0; xO < stride; ++xO)
                        {
                        if (Y[offsetY] < I[offsetI + yO * sizeX * sizeC + xO * sizeC])
                            {
                            Y[offsetY] = I[offsetI + yO * sizeX * sizeC + xO * sizeC];
                            }
                        }
                    }
                }

            }
        }

    return Y;
}
