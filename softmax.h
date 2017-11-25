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

///
/// IMPLEMENTATION
///

template<unsigned int size>
Softmax<size>::
Softmax()
{

}

template<unsigned int size>
void Softmax<size>::
apply(layerOut_t *I)
{
    // Finds max element
    layerOut_t maxElement = I[0];
    for (unsigned int i = 1; i < size; ++i)
        {
        if (maxElement < I[i])
            {
            maxElement = I[i];
            }
        }

    // Exponential
    softSE_t sumExp = 0;
    for (unsigned int i = 0; i < size; ++i)
        {
        Y[i] = expFP(I[i] - maxElement);
        sumExp += Y[i];
        }

    for (unsigned int i = 0; i < size; ++i)
        {
        Y[i] /= sumExp;
        }
}

template<unsigned int size>
softE_t Softmax<size>::
expFP(const layerOut_t &x) const
{
    //TODO Write exponential in fixed point
    return exp(x.to_double());
}
#endif // SOFTMAX_H
