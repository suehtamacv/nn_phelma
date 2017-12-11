#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H

#include "fixedpointvariables.h"
#include <ac_channel.h>

template<unsigned int size>
struct memInterface
{
    layerOut_t Y[size];
};

#endif // MEMINTERFACE_H
