#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
class Perceptron
{
public:
    Perceptron(const perceptronKernel_t K[sizeKx * sizeKy], const perceptronBias_t B[sizeX * sizeKy],
               layerOut_t *pY);

    void apply(layerOut_t*);

    ///
    /// \brief Y
    /// Size : sizeKx * sizeX
    ///
    layerOut_t *Y;

private:
    ///
    /// \brief K
    /// Size : sizeKy * sizeKx
    ///
    const perceptronKernel_t *K;

    ///
    /// \brief B
    /// Size : sizeKx * sizeX
    const perceptronBias_t *B;

};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
Perceptron<sizeKx, sizeKy, sizeX>::
Perceptron(const perceptronKernel_t *K, const perceptronBias_t *B, layerOut_t *pY) : Y(pY), K(K), B(B)
{

}

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
void Perceptron<sizeKx, sizeKy, sizeX>::
apply(layerOut_t *I)
{

    for (unsigned int kY = 0; kY < sizeKy; ++kY)
        {
        for (unsigned int kX = 0; kX < sizeKx; ++kX)
            {
            for (unsigned iX = 0; iX < sizeX; ++iX)
                {
                if (kY == 0)
                    {
                    Y[kX * sizeX + iX] = K[kY * sizeKx + kX] * I[kY * sizeX + iX] + B[kX * sizeX + iX];
                    }
                else
                    {
                    Y[kX * sizeX + iX] += K[kY * sizeKx + kX] * I[kY * sizeX + iX];
                    }
                }
            }
        }

}

#endif // PERCEPTRON_H
