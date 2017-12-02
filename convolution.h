#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"

#define overlap  2
#define tileSize 4

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
class Convolution
{
public:
    Convolution(const convKernel_t K[sizeC * sizeL * 3 * 3], layerOut_t* pY);

    layerOut_t* apply(layerOut_t *I);

    ///
    /// \brief Y is the output matrix.
    /// Size : sizeY * sizeX * sizeL
    ///
    layerOut_t* Y;

private:
    ///
    /// \brief K is the convolution kernel.
    /// Size : sizeC * sizeL * 3 * 3
    ///
    const convKernel_t *K;

    void calculateG(convKernel_t *, const unsigned int);
    void calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
                    const unsigned int yI, const unsigned int cI);
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
Convolution<sizeX, sizeY, sizeC, sizeL>::
Convolution(const convKernel_t K[sizeC * sizeL * 3 * 3], layerOut_t *pY) : Y(pY), K(K)
{

}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
layerOut_t *Convolution<sizeX, sizeY, sizeC, sizeL>::apply(layerOut_t *I)
{
    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];

    // X coordinate
loopConvXi:
    for (unsigned int xI = 0; xI < sizeX; xI += overlap)
        {
        // Y coordinate
loopConvYi:
        for (unsigned int yI = 0; yI < sizeY; yI += overlap)
            {

            // Output channels
loopConvOutChannel:
            for (unsigned int l = 0; l < sizeL; l++)
                {
                // Convolution result in transform space
                convM_t M[tileSize * tileSize] = {0};

                // Input channels
loopConvInChannel:
                for (unsigned int c = 0; c < sizeC; c++)
                    {
                    calculateG(G, (l * sizeC + c) * 9);
                    calculateD(I, D, xI, yI, c);

loopTile:
                    for (unsigned i = 0; i < tileSize * tileSize; ++i)
                        {
                        // If it is the first iteration, initialization of M
                        if (c == 0)
                            {
                            M[i] = D[i] * G[i];
                            }
                        else
                            {
                            M[i] += D[i] * G[i];
                            }
                        }
                    }

                // Transform to image space
                    {
                    convTemp_t temp[2][4] = {0};

loopInverseTransform:
                    for (unsigned int i = 0; i < 4; ++i)
                        {
                        temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                        temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                        }

loopInverseTransform2:
                    for (unsigned int j = 0; j < 2; ++j)
                        {
                        // If is the first iteration, initialization of Y
                        if (l == 0)
                            {
                            Y[yI * sizeX * sizeL + (xI + j) * sizeL + l] =
                                temp[j][0] + temp[j][1] + temp[j][2];
                            Y[(yI + 1) * sizeX * sizeL + (xI + j) * sizeL + l] =
                                temp[j][1] - temp[j][2] - temp[j][3];
                            }
                        else
                            {
                            Y[yI * sizeX * sizeL + (xI + j) * sizeL + l] +=
                                temp[j][0] + temp[j][1] + temp[j][2];
                            Y[(yI + 1) * sizeX * sizeL + (xI + j) * sizeL + l] +=
                                temp[j][1] - temp[j][2] - temp[j][3];
                            }
                        }
                    } // End transformation

                }
            }
        }

    return Y;
}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void Convolution<sizeX, sizeY, sizeC, sizeL>::
calculateG(convKernel_t *G, const unsigned int offsetG)
{
    G[0] = K[offsetG + 0];
    G[1] = (K[offsetG + 0] + K[offsetG + 1] + K[offsetG + 2]) >> 1;
    G[2] = (K[offsetG + 0] - K[offsetG + 1] + K[offsetG + 2]) >> 1;
    G[3] = K[offsetG + 2];

    G[4] = (K[offsetG + 0] + K[offsetG + 3] + K[offsetG + 6]) >> 1;
    G[5] = (K[offsetG + 0] + K[offsetG + 1] + K[offsetG + 2]
            + K[offsetG + 3] + K[offsetG + 4] + K[offsetG + 5]
            + K[offsetG + 6] + K[offsetG + 7] + K[offsetG + 8]) >> 2;
    G[6] = (K[offsetG + 0] - K[offsetG + 1] + K[offsetG + 2]
            + K[offsetG + 3] - K[offsetG + 4] + K[offsetG + 5]
            + K[offsetG + 6] - K[offsetG + 7] + K[offsetG + 8]) >> 2;
    G[7] = (K[offsetG + 2] + K[offsetG + 5] + K[offsetG + 8]) >> 1;

    G[8] = (K[offsetG + 0] - K[offsetG + 3] + K[offsetG + 6]) >> 1;
    G[9] = (K[offsetG + 0] + K[offsetG + 1] + K[offsetG + 2]
            - K[offsetG + 3] - K[offsetG + 4] - K[offsetG + 5]
            + K[offsetG + 6] + K[offsetG + 7] + K[offsetG + 8]) >> 2;
    G[10] = (K[offsetG + 0] - K[offsetG + 1] + K[offsetG + 2]
             - K[offsetG + 3] + K[offsetG + 4] - K[offsetG + 5]
             + K[offsetG + 6] - K[offsetG + 7] + K[offsetG + 8]) >> 2;
    G[11] = (K[offsetG + 2] - K[offsetG + 5] + K[offsetG + 8]) >> 1;

    G[12] = K[offsetG + 6];
    G[13] = (K[offsetG + 6] + K[offsetG + 7] + K[offsetG + 8]) >> 1;
    G[14] = (K[offsetG + 6] - K[offsetG + 7] + K[offsetG + 8]) >> 1;
    G[15] = K[offsetG + 8];
}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void Convolution<sizeX, sizeY, sizeC, sizeL>::
calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
           const unsigned int yI, const unsigned int cI)
{
#define T(y, x) \
    I[(yI + y) * sizeX * sizeC + (xI + x) * sizeC + cI]

    const bool xNotBorder = xI < sizeX - overlap;
    const bool yNotBorder = yI < sizeY - overlap;

    if (xNotBorder && yNotBorder)
        {
        D[0] = T(0, 0) - T(0, 2) - T(2, 0) + T(2, 2);
        D[1] = T(0, 1) + T(0, 2) - T(2, 1) - T(2, 2);
        D[2] = -T(0, 1) + T(0, 2) + T(2, 1) - T(2, 2);
        D[3] = T(0, 1) - T(0, 3) - T(2, 1) + T(2, 3);

        D[4] = T(1, 0) - T(1, 2) + T(2, 0) - T(2, 2);
        D[5] = T(1, 1) + T(1, 2) + T(2, 1) + T(2, 2);
        D[6] = -T(1, 1) + T(1, 2) - T(2, 1) + T(2, 2);
        D[7] = T(1, 1) - T(1, 3) + T(2, 1) - T(2, 3);

        D[8] = -T(1, 0) + T(1, 2) + T(2, 0) - T(2, 2);
        D[9] = -T(1, 1) - T(1, 2) + T(2, 1) + T(2, 2);
        D[10] = T(1, 1) - T(1, 2) - T(2, 1) + T(2, 2);
        D[11] = -T(1, 1) + T(1, 3) + T(2, 1) - T(2, 3);

        D[12] = T(1, 0) - T(1, 2) - T(3, 0) + T(3, 2);
        D[13] = T(1, 1) + T(1, 2) - T(3, 1) - T(3, 2);
        D[14] = -T(1, 1) + T(1, 2) + T(3, 1) - T(3, 2);
        D[15] = T(1, 1) - T(1, 3) - T(3, 1) + T(3, 3);
        }
    else
        {
        for (unsigned int i = 0; i < 16; ++i)
            {
            D[i] = 0;
            }
        }

#undef T
}

#endif // CONVOLUTION_H
