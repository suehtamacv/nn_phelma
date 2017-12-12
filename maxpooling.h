#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
class MaxPooling
{
public:
    typedef memInterface<sizeY * sizeX * sizeC> memInStruct;
    typedef memInterface < (sizeY / stride) * (sizeX / stride) * sizeC > memOutStruct;

    MaxPooling(const std::string name);

    void apply(ac_channel<memInStruct> &I, ac_channel<memOutStruct> &Y);

private:
    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
MaxPooling<stride, poolSize, sizeX, sizeY, sizeC>::
MaxPooling(const std::string name) :
    name(name)
{

}

#pragma design
template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void MaxPooling<stride, poolSize, sizeX, sizeY, sizeC>::
apply(ac_channel<memInStruct> &I, ac_channel<memOutStruct> &Y)
{
    memInStruct  bufferI = I.read();
    memOutStruct bufferY;

#ifdef __HWC__
#define T(x, y) \
    bufferI.Y[(yI + (y)) * sizeX * sizeC + (xI + (x)) * sizeC + cI]
#else
#define T(x, y) \
    bufferI.Y[cI * sizeY * sizeX + (yI + (y)) * sizeX + (xI + (x))]
#endif

    unsigned int nxI = 0, nyI = 0;
    layerOut_t tempMax = 0;
    layerOut_t tempVal = 0;

loopChannels:
    for (unsigned int cI = 0; cI < sizeC; ++cI)
        {
        nyI = 0;
loopY:
        for (unsigned int yI = 0; yI < sizeY; yI += stride, ++nyI)
            {
            nxI = 0;
loopX:
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

		tempVal = T(0, 0);

                tempMax = tempVal;
loopYBlock:
                for (unsigned int yO = 0; yO < yLim; ++yO)
                    {
loopXBlock:
                    for (unsigned int xO = 0; xO < xLim; ++xO)
                        {
			tempVal = T(xO, yO);
                        if (tempMax < tempVal)
                            {
                            tempMax = tempVal;
                            }
                        }
                    }

                bufferY.Y[offsetY] = tempMax;
                }

            }
        }

    Y.write(bufferY);

#undef T
}

#undef newSizeX
#undef newSizeY

#endif // MAXPOOLING_H
