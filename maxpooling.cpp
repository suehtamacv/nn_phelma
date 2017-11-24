#include "maxpooling.h"

MaxPooling::MaxPooling(unsigned int stride) : stride(stride)
{

}

layerOut_t *MaxPooling::apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC)
{
    const unsigned int newSizeX = sizeX / stride;
    const unsigned int newSizeY = sizeY / stride;

    unsigned int nxI = 0, nyI = 0;

    layerOut_t* Y = new layerOut_t[newSizeY * newSizeX * sizeC];

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
