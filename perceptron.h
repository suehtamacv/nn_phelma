#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
class Perceptron
{
public:
    Perceptron(const matrixProdKernel_t K[sizeKx * sizeKy], layerOut_t *pY);

    void apply(layerOut_t*);

    ///
    /// \brief Y
    /// Size : sizeKx * sizeY
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

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
Perceptron<sizeKx, sizeKy, sizeY>::
Perceptron(const matrixProdKernel_t K[], layerOut_t *pY) : Y(pY), K(K)
{

}

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
void Perceptron<sizeKx, sizeKy, sizeY>::
apply(layerOut_t *I)
{
    for (unsigned int iX = 0; iX < sizeKx; ++iX)
        {
        for (unsigned int iY = 0; iY < sizeY; ++iY)
            {
            for (unsigned int iZ = 0; iZ < sizeKy; iZ++)
                {
                Y[iX * sizeY + iY] += I[iX * sizeY + iZ] * K[iZ * sizeKy + iY];
                }
            }
        }
}

#endif // PERCEPTRON_H
