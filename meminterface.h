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

typedef memInterface<24 * 24 * 3> conv1_In_t;
typedef memInterface<24 * 24 * 64> conv1_Out_t;
typedef memInterface<12 * 12 * 64> conv2_In_t;
typedef memInterface<12 * 12 * 32> conv2_Out_t;
typedef memInterface<6 * 6 * 32> conv3_In_t;
typedef memInterface<6 * 6 * 20> conv3_Out_t;
typedef memInterface<3 * 3 * 20> percep4_In_t;
typedef memInterface<10> percep4_Out_t;


#endif // MEMINTERFACE_H
