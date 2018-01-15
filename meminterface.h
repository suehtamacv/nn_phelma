#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H

#include "fixedpointvariables.h"
#include "nnarrays.h"
#include <ac_channel.h>

template<unsigned int size>
struct lineBlockInterface
{
    layerOutBlock_t Y[size / BLOCK_HEIGHT];
};

template<unsigned int size>
struct memBlockInterface
{
    layerOutBlock_t Y[size / PIXEL_PER_BLOCK];
};

template<unsigned int size>
struct memInterface
{
    pixel_t Y[size];
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

typedef memBlockInterface<INPUT_SIZE> conv_In_t;
typedef memInterface<OUTPUT_SIZE> conv_Out_t;



#endif // MEMINTERFACE_H
