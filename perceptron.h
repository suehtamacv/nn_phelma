#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
class Perceptron
{
public:
    Perceptron(const matrixProdKernel_t K[sizeKx * sizeKy], layerOut_t *pY);

    void apply(layerOut_t*);

    ///
    /// \brief Y
    /// Size : sizeX * sizeKy
    ///
    layerOut_t *Y;

private:
    ///
    /// \brief K
    /// Size : sizeKx * sizeKy
    ///
    const matrixProdKernel_t *K;
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
Perceptron<sizeKx, sizeKy, sizeX>::
Perceptron(const matrixProdKernel_t *K, layerOut_t *pY) : Y(pY), K(K)
{
loopInitOutput:
    for (unsigned int i = 0; i < sizeKx * sizeX; ++i)
        {
        Y[i] = 0;
        }
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
                Y[kX * sizeX + iX] += K[kY * sizeKx + kX] * I[kY * sizeX + iX];
                }
            }
        }

}

#endif // PERCEPTRON_H
