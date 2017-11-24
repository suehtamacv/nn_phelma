#include "softmax.h"

Softmax::Softmax()
{

}

uint8* Softmax::apply(int8 *I, const unsigned int sizeX)
{
    uint8* Y = new uint8[sizeX];

    // Finds max element
    int8 maxElement = I[0];
    for (unsigned int i = 1; i < sizeX; ++i)
        {
        if (maxElement < I[i])
            {
            maxElement = I[i];
            }
        }

    // Exponential
    uint16 sumExp = 0;
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

int8 Softmax::expFP(const int8 &x)
{
    std::cout << "EXP " << x.to_int() << " = " << exp(x.to_int()) << std::endl;
    return (int8) exp(x.to_int());
}
