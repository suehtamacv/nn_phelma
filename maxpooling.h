#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include <fixedpointvariables.h>

template<unsigned int stride, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
class MaxPooling
{
public:
    MaxPooling();

    void apply(layerOut_t *I);

    layerOut_t Y[(sizeX / stride) * (sizeY / stride) * sizeC];

private:
    const unsigned int newSizeX;
    const unsigned int newSizeY;
};

///
/// IMPLEMENTATION
///

template<unsigned int stride, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
MaxPooling<stride, sizeX, sizeY, sizeC>::
MaxPooling() :
    newSizeX(sizeX / stride),
    newSizeY(sizeY / stride)
{

}

template<unsigned int stride, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void MaxPooling<stride, sizeX, sizeY, sizeC>::
apply(layerOut_t *I)
{
    unsigned int nxI = 0, nyI = 0;

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
}

#endif // MAXPOOLING_H
