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

///
/// IMPLEMENTATION
///

template<unsigned int size>
ReLU<size>::ReLU()
{

}

template<unsigned int size>
void ReLU<size>::apply(layerOut_t *I)
{
    for (unsigned int i = 0; i < size; ++i)
        {
        Y[i] = I[i] > 0 ? I[i] : 0;
        }
}

#endif // RELU_H


