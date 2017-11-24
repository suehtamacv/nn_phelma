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

#include "maxpooling.tpp"

#endif // MAXPOOLING_H
