#include "softmax.h"

static const softE_t log_2 = 0.6931471805599453;

Softmax::Softmax()
{

}

layerOut_t *Softmax::apply(layerOut_t *I, const unsigned int sizeX)
{
    layerOut_t* Y = new layerOut_t[sizeX];

    // Finds max element
    layerOut_t maxElement = I[0];
    for (unsigned int i = 1; i < sizeX; ++i)
        {
        if (maxElement < I[i])
            {
            maxElement = I[i];
            }
        }

    // Exponential
    softSE_t sumExp = 0;
    for (unsigned int i = 0; i < sizeX; ++i)
        {
        Y[i] = expFP(I[i] - maxElement);
        sumExp += Y[i];
        }

    for (unsigned int i = 0; i < sizeX; ++i)
        {
        Y[i] /= sumExp;
        }

    return Y;
}

softE_t Softmax::expFP(const layerOut_t &x) const
{
    //TODO Write exponential in fixed point
    return exp(x.to_double());
}
