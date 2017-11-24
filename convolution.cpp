#include "convolution.h"

#define tileSize 4
#define overlap  2

Convolution::Convolution(const kernel_t *K,
                         unsigned int kernelSizeC,
                         unsigned int kernelSizeL) :
    K(K),
    kernelSizeC(kernelSizeC),
    kernelSizeL(kernelSizeL)
{

}

layerOut_t *Convolution::apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY)
{
    kernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    layerOut_t* Y = new layerOut_t[sizeY * sizeX * kernelSizeL];

    // Initialize R
    std::fill(Y, Y + sizeY * sizeX * kernelSizeL, 0);

    // X coordinate
    for (unsigned int xI = 0; xI < sizeX; xI += overlap)
        {
        // Y coordinate
        for (unsigned int yI = 0; yI < sizeY; yI += overlap)
            {

            // Output channels
            for (unsigned int l = 0; l < kernelSizeL; l++)
                {
                // Convolution result in transform space
                convM_t M[tileSize * tileSize] = {0};
                std::fill(M, M + tileSize * tileSize, 0);

                // Input channels
                for (unsigned int c = 0; c < kernelSizeC; c++)
                    {
                    calculateG(G, (l * kernelSizeC + c) * 9);
                    calculateD(I, D, xI, yI, c, sizeX, sizeY, kernelSizeC);

                    for (unsigned i = 0; i < tileSize * tileSize; ++i)
                        {
                        M[i] += D[i] * G[i];
                        }
                    }

                // Transform to image space
                    {
                    convTemp_t temp[2][4] = {0};
                    for (unsigned int i = 0; i < 4; ++i)
                        {
                        temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                        temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                        }

                    for (unsigned int j = 0; j < 2; ++j)
                        {
                        Y[yI * sizeX * kernelSizeL + (xI + j) * kernelSizeL + l] +=
                            temp[j][0] + temp[j][1] + temp[j][2];
                        Y[(yI + 1) * sizeX * kernelSizeL + (xI + j) * kernelSizeL + l] +=
                            temp[j][1] - temp[j][2] - temp[j][3];
                        }
                    } // End transformation

                }
            }
        }

    return Y;
}

void Convolution::calculateG(kernel_t *G, const unsigned int offsetG)
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

void Convolution::calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
                             const unsigned int yI, const unsigned int cI,
                             const unsigned int sizeX, const unsigned int sizeY,
                             const unsigned int sizeC)
{
#define T(y, x) \
    I[(yI + y) * sizeX * sizeC + (xI + x) * sizeC + cI]

    bool xNotBorder = xI < sizeX - overlap;
    bool yNotBorder = yI < sizeY - overlap;

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
        std::fill(D, D + 16, 0);
        }

#undef T
}

