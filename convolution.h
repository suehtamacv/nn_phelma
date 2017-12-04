#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"

#define overlap  2
#define tileSize 4

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
class Convolution
{
public:
    Convolution(const convKernel_t K[sizeC * sizeL * 3 * 3], const convBias_t B[sizeL], layerOut_t* pY);

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

    ///
    /// \brief B are the biases.
    /// Size : sizeL
    ///
    const convBias_t *B;

    void calculateG(convKernel_t *, const convKernel_t *);
    void calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
                    const unsigned int yI, const unsigned int cI);
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
Convolution<sizeX, sizeY, sizeC, sizeL>::
Convolution(const convKernel_t K[sizeC * sizeL * 3 * 3], const convBias_t B[sizeL], layerOut_t *pY) :
    Y(pY), K(K), B(B)
{

}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
layerOut_t *Convolution<sizeX, sizeY, sizeC, sizeL>::apply(layerOut_t *I)
{
    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];

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

                // Input channels
loopConvInChannel:
                for (unsigned int c = 0; c < sizeC; c++)
                    {
                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    calculateG(G, K + ((l * sizeC + c) * 9));
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
                        //
                        if (l == 0)
                            {
                            Y[l * sizeY * sizeX + yI * sizeX + (xI + j)] =
                                temp[j][0] + temp[j][1] + temp[j][2] + B[l];
                            Y[l * sizeY * sizeX + (yI + 1) * sizeX + (xI + j)] =
                                temp[j][1] - temp[j][2] - temp[j][3] + B[l];
                            }
                        else
                            {
                            Y[l * sizeY * sizeX + yI * sizeX + (xI + j)] +=
                                temp[j][0] + temp[j][1] + temp[j][2];
                            Y[l * sizeY * sizeX + (yI + 1) * sizeX + (xI + j)] +=
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
calculateG(convKernel_t *G, const convKernel_t *K)
{
    G[0]  = K[0];
    G[1]  = (K[0] + K[1] + K[2]) >> 1;
    G[2]  = (K[0] - K[1] + K[2]) >> 1;
    G[3]  = K[2];

    G[4]  = (K[0] + K[3] + K[6]) >> 1;
    G[5]  = (K[0] + K[1] + K[2] + K[3] + K[4] + K[5] + K[6] + K[7] + K[8]) >> 2;
    G[6]  = (K[0] - K[1] + K[2] + K[3] - K[4] + K[5] + K[6] - K[7] + K[8]) >> 2;
    G[7]  = (K[2] + K[5] + K[8]) >> 1;

    G[8]  = (K[0] - K[3] + K[6]) >> 1;
    G[9]  = (K[0] + K[1] + K[2] - K[3] - K[4] - K[5] + K[6] + K[7] + K[8]) >> 2;
    G[10] = (K[0] - K[1] + K[2] - K[3] + K[4] - K[5] + K[6] - K[7] + K[8]) >> 2;
    G[11] = (K[2] - K[5] + K[8]) >> 1;

    G[12] = K[6];
    G[13] = (K[6] + K[7] + K[8]) >> 1;
    G[14] = (K[6] - K[7] + K[8]) >> 1;
    G[15] = K[8];
}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void Convolution<sizeX, sizeY, sizeC, sizeL>::
calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
           const unsigned int yI, const unsigned int cI)
{
#define T(y, x) \
    I[cI * sizeY * sizeX + (yI + y) * sizeX + (xI + x)]

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
