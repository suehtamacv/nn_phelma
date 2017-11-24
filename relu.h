#ifndef RELU_H
#define RELU_H

#include <fixedpointvariables.h>

class ReLU
{
public:
    ReLU();

    layerOut_t *apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC);
};

#endif // RELU_H


