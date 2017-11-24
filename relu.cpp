#include "relu.h"

ReLU::ReLU()
{

}

layerOut_t *ReLU::apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC)
{
    layerOut_t* Y = new layerOut_t[sizeY * sizeX * sizeC]();

    for (unsigned int i = 0; i < sizeY * sizeX * sizeC; ++i)
        {
        Y[i] = I[i] > 0 ? I[i] : 0;
        }

    return Y;
}

