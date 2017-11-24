#ifndef RELU_H
#define RELU_H

#include <fixedpointvariables.h>

template<unsigned int size>
class ReLU
{
public:
    ReLU();

    void apply(layerOut_t *I);

    layerOut_t Y[size];
};

#include "relu.tpp"

#endif // RELU_H


