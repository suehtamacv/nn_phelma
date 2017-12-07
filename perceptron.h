#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
class Perceptron
{
public:
    Perceptron(const std::string name, const perceptronKernel_t K[sizeKx * sizeKy],
               const perceptronBias_t B[sizeX * sizeKx],
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

    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
Perceptron<sizeKx, sizeKy, sizeX>::
Perceptron(const std::string name, const perceptronKernel_t *K, const perceptronBias_t *B,
           layerOut_t *pY) : Y(pY),
    K(K), B(B), name(name)
{

}

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeX>
void Perceptron<sizeKx, sizeKy, sizeX>::
apply(layerOut_t *I)
{

    for (unsigned int iKy = 0; iKy < sizeKy; ++iKy)
        {
        for (unsigned int iKx = 0; iKx < sizeKx; ++iKx)
            {
            for (unsigned iIx = 0; iIx < sizeX; ++iIx)
                {
                if (iKy == 0)
                    {
                    Y[iKy * sizeX + iIx] = K[iKy * sizeKx + iKx] * I[iKx * sizeX + iIx] + B[iKy * sizeX + iIx];
                    }
                else
                    {
                    Y[iKy * sizeX + iIx] += K[iKy * sizeKx + iKx] * I[iKx * sizeX + iIx];
                    }
                }
            }
        }

}

#endif // PERCEPTRON_H
