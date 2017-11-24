#ifndef SOFTMAX_H
#define SOFTMAX_H

#include <fixedpointvariables.h>

template<unsigned int size>
class Softmax
{
public:
    Softmax();

    void apply(layerOut_t *I);

    layerOut_t Y[size];

private:
    inline softE_t expFP(const layerOut_t &x) const;
};

#include "softmax.tpp"

#endif // SOFTMAX_H
