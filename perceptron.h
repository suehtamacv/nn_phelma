#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"

template<unsigned int sizeKx, unsigned int sizeKy>
class Perceptron
{
public:
    Perceptron(const std::string name, const perceptronKernel_t K[sizeKx * sizeKy],
               const perceptronBias_t B[sizeKx],
               layerOut_t *pY);

    void apply(layerOut_t*);

    ///
    /// \brief Y
    /// Size : sizeKx
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
    /// Size : sizeKx
    const perceptronBias_t *B;

    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy>
Perceptron<sizeKx, sizeKy>::
Perceptron(const std::string name, const perceptronKernel_t *K, const perceptronBias_t *B,
           layerOut_t *pY) : Y(pY),
    K(K), B(B), name(name)
{

}

template<unsigned int sizeKx, unsigned int sizeKy>
void Perceptron<sizeKx, sizeKy>::
apply(layerOut_t *I)
{
    for (unsigned int iKy = 0; iKy < sizeKy; ++iKy)
        {
        for (unsigned int iKx = 0; iKx < sizeKx; ++iKx)
            {
            if (iKx == 0)
                {
                Y[iKy] = K[iKy * sizeKx + iKx] * I[iKx] + B[iKy];
                }
            else
                {
                Y[iKy] += K[iKy * sizeKx + iKx] * I[iKx];
                }
            }
#ifdef __STAT__
        std::cout << name << " Y " << Y[iKy] << std::endl;
#endif
        }

}

#endif // PERCEPTRON_H
