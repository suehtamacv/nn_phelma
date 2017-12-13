#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H

#include "fixedpointvariables.h"
#include <ac_channel.h>

template<unsigned int size>
struct memInterface
{
    layerOut_t Y[size];
};

template<unsigned int size>
struct convKernelInterface
{
    convKernel_t K[size];
};

template<unsigned int size>
struct convBiasInterface
{
    convBias_t B[size];
};

template<unsigned int size>
struct perceptronKernelInterface
{
    perceptronKernel_t K[size];
};

template<unsigned int size>
struct perceptronBiasInterface
{
    perceptronBias_t B[size];
};

#endif // MEMINTERFACE_H
