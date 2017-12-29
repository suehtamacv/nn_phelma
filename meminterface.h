#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H

#include "fixedpointvariables.h"
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

typedef lineBlockInterface<24 * 3> conv1_line_In_t;
typedef lineBlockInterface<24 * 64> conv1_line_Out_t;
typedef lineBlockInterface<12 * 64> conv2_line_In_t;
typedef lineBlockInterface<12 * 32> conv2_line_Out_t;
typedef lineBlockInterface<6 * 32> conv3_line_In_t;
typedef lineBlockInterface<6 * 20> conv3_line_Out_t;
typedef memBlockInterface<3 * 3 * 20> percep4_In_t;
typedef memInterface<10> percep4_Out_t;


#endif // MEMINTERFACE_H
