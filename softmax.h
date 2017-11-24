#ifndef SOFTMAX_H
#define SOFTMAX_H

#include <fixedpointvariables.h>

class Softmax
{
public:
    Softmax();
    layerOut_t* apply(layerOut_t *I, const unsigned int sizeX);

private:
    inline softE_t expFP(const layerOut_t &x) const;
};

#endif // SOFTMAX_H
