#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"

template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
class MaxPooling
{
public:
    MaxPooling(const std::string name, layerOut_t*);

    void apply(layerOut_t *I);

    layerOut_t *Y;

private:
    const unsigned int newSizeX;
    const unsigned int newSizeY;

    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
MaxPooling<stride, poolSize, sizeX, sizeY, sizeC>::
MaxPooling(const std::string name, layerOut_t* pY) :
    Y(pY),
    newSizeX(sizeX / stride),
    newSizeY(sizeY / stride),
    name(name)
{

}

template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void MaxPooling<stride, poolSize, sizeX, sizeY, sizeC>::
apply(layerOut_t *I)
{
#ifdef __HWC__
#define T(x, y) \
    I[(yI + (y)) * sizeX * sizeC + (xI + (x)) * sizeC + cI]
#else
#define T(x, y) \
    I[cI * sizeY * sizeX + (yI + (y)) * sizeX + (xI + (x))]
#endif

    unsigned int nxI = 0, nyI = 0;

    for (unsigned int cI = 0; cI < sizeC; ++cI)
        {
        nyI = 0;
        for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
            {
            nxI = 0;
            for (unsigned int xI = 0; xI < sizeX; xI += stride, ++nxI)
                {

#ifdef __HWC__
                const unsigned int offsetY = nyI * newSizeX * sizeC + nxI * sizeC + cI;
#else
                const unsigned int offsetY = cI * newSizeY * newSizeX + nyI * newSizeX + nxI;
#endif

                const bool xBorder = xI + poolSize >= sizeX;
                const bool yBorder = yI + poolSize >= sizeY;

                const unsigned int xLim = xBorder ? sizeX - xI : poolSize;
                const unsigned int yLim = yBorder ? sizeY - yI : poolSize;

                Y[offsetY] = T(0, 0);
                for (unsigned int yO = 0; yO < yLim; ++yO)
                    {
                    for (unsigned int xO = 0; xO < xLim; ++xO)
                        {
                        if (Y[offsetY] < T(xO, yO))
                            {
                            Y[offsetY] = T(xO, yO);
                            }
                        }
                    }
                }

            }
        }
#undef T
}

#endif // MAXPOOLING_H
