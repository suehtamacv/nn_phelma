#include "matrixmultiply.h"

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
MatrixMultiply<sizeKx, sizeKy, sizeY>::
MatrixMultiply(const matrixProdKernel_t K[])
{
    std::fill(Y, Y + sizeKx * sizeY, 0);
    for (unsigned int i = 0; i < sizeKx * sizeKy; ++i)
        {
        (this->K)[i] = K[i];
        }
}

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
void MatrixMultiply<sizeKx, sizeKy, sizeY>::
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
