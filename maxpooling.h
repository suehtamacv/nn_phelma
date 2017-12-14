#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

#pragma design
template<unsigned int stride, unsigned int poolSize, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC>
void maxPooling_apply(ac_channel<memInterface<sizeY * sizeX * sizeC> > &I,
                      ac_channel < memInterface < newSizeX * newSizeY * sizeC > > &Y)
{
    memInterface<sizeY * sizeX * sizeC> bufferI = I.read();
    memInterface < newSizeX * newSizeY * sizeC > bufferY;

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
                tempVal = T(0, 0);
                tempMax = tempVal;
loopYBlock:
                for (unsigned int yO = 0; yO < poolSize; ++yO)
                    {
                    const bool yBorder = yI + yO >= sizeY;

loopXBlock:
                    for (unsigned int xO = 0; xO < poolSize; ++xO)
                        {
                        const bool xBorder = xI + xO >= sizeX;

                        tempVal = (!yBorder && !xBorder) ? T(xO, yO) : tempVal;
                        tempMax = (tempMax < tempVal) ? tempVal : tempMax;
                        }
                    }

#ifdef __HWC__
                bufferY.Y[nyI * newSizeX * sizeC + nxI * sizeC + cI] = tempMax;
#else
                bufferY.Y[cI * newSizeY * newSizeX + nyI * newSizeX + nxI] = tempMax;
#endif
                }

            }
        }

    Y.write(bufferY);

#undef T
}

#undef newSizeX
#undef newSizeY

#endif // MAXPOOLING_H
