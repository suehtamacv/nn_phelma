#include "matrixmultiply.h"

MatrixMultiply::MatrixMultiply(const uint8 *K,
                               const unsigned int sizeKx,
                               const unsigned int sizeKy) : K(K), sizeKx(sizeKx), sizeKy(sizeKy)
{

}

uint8* MatrixMultiply::apply(uint8 *I, const unsigned int sizeY)
{
    uint8 *Y = new uint8[sizeKx * sizeY];
    std::fill(Y, Y + sizeKx * sizeY, 0);

    for (unsigned int iX = 0; iX < sizeKx; ++iX)
        {
        for (unsigned int iY = 0; iY < sizeY; ++iY)
            {
            for (int iZ = 0; iZ < sizeKy; iZ++)
                {
                Y[iX * sizeY + iY] += I[iX * sizeY + iZ] * K[iZ * sizeKy + iY];
                }
            }

        }
}
