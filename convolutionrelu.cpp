#include "convolutionrelu.h"
#include "kernels.h"
#include "nnarrays.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

void conv_apply(ac_channel<conv_In_t> &I, ac_channel<conv_Out_t> &Y, bool selectKernel)
{
#define sizeX HEIGHT
#define sizeY WIDTH
#define sizeC 3
#define sizeL 3

    conv_In_t  bufferI = I.read();
    conv_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];

    // Y coordinate
loopConvYi:
    for (unsigned int yI = 0; yI < sizeY - overlap; yI += overlap)
        {

        // X coordinate
loopConvXi:
        for (unsigned int xI = 0; xI < sizeX - overlap; xI += overlap)
            {

            // Output channels
loopConvOutChannel:
            for (unsigned int lI = 0; lI < sizeL; lI++)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeY, sizeC>(bufferI, Block, xI, yI, cI);

                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    if (selectKernel)
                        {
                        calculateG(G, kernelIdentity + ((lI * sizeC + cI) * 9));
                        }
                    else
                        {
                        calculateG(G, kernelGauss + ((lI * sizeC + cI) * 9));
                        }
                    calculateD(Block, D);

loopTile:
                    for (unsigned i = 0; i < tileSize * tileSize; ++i)
                        {
                        // If it is the first iteration, initialization of M
                        if (cI == 0)
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
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                    }

                unsigned int index = lI * ((sizeY - BLOCK_HEIGHT) / BLOCK_HEIGHT) * ((sizeX - BLOCK_WIDTH) / BLOCK_WIDTH)
                                     + (yI / BLOCK_WIDTH) * ((sizeX - BLOCK_WIDTH) / BLOCK_WIDTH)
                                     + (xI / BLOCK_HEIGHT);

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    bufferY.Y[index][j] = temp[j][0] + temp[j][1] + temp[j][2];
                    bufferY.Y[index][2 + j] = temp[j][1] - temp[j][2] - temp[j][3];
                    }
                // End transformation
                }
            }
        }

    Y.write(bufferY);

#undef sizeX
#undef sizeY
#undef sizeC
#undef sizeL
}

void calculateG(convKernel_t (&G)[16], const convKernel_t *K)
{
    // Flipping convolution kernel
#define K(y, x) K[y * 3 + x]

    G[0]  = K(0, 0);
    G[1]  = (K(0, 0) + K(0, 1) + K(0, 2)) / 2;
    G[2]  = (K(0, 0) - K(0, 1) + K(0, 2)) / 2;
    G[3]  = K(0, 2);

    G[4]  = (K(0, 0) + K(1, 0) + K(2, 0)) / 2;
    G[5]  = (K(0, 0) + K(0, 1) + K(0, 2) + K(1, 0) + K(1, 1) + K(1, 2) + K(2, 0) + K(2, 1) + K(2, 2)) / 4;
    G[6]  = (K(0, 0) - K(0, 1) + K(0, 2) + K(1, 0) - K(1, 1) + K(1, 2) + K(2, 0) - K(2, 1) + K(2, 2)) / 4;
    G[7]  = (K(0, 2) + K(1, 2) + K(2, 2)) / 2;

    G[8]  = (K(0, 0) - K(1, 0) + K(2, 0)) / 2;
    G[9]  = (K(0, 0) + K(0, 1) + K(0, 2) - K(1, 0) - K(1, 1) - K(1, 2) + K(2, 0) + K(2, 1) + K(2, 2)) / 4;
    G[10] = (K(0, 0) - K(0, 1) + K(0, 2) - K(1, 0) + K(1, 1) - K(1, 2) + K(2, 0) - K(2, 1) + K(2, 2)) / 4;
    G[11] = (K(0, 2) - K(1, 2) + K(2, 2)) / 2;

    G[12] = K(2, 0);
    G[13] = (K(2, 0) + K(2, 1) + K(2, 2)) / 2;
    G[14] = (K(2, 0) - K(2, 1) + K(2, 2)) / 2;
    G[15] = K(2, 2);

#undef K
}

void calculateD(pixel_t (&Block)[16], convD_t (&D)[16])
{
#define B(y, x) \
    Block[y * tileSize + x]

    D[0] = B(0, 0) - B(0, 2) - B(2, 0) + B(2, 2);
    D[1] = B(0, 1) + B(0, 2) - B(2, 1) - B(2, 2);
    D[2] = -B(0, 1) + B(0, 2) + B(2, 1) - B(2, 2);
    D[3] = B(0, 1) - B(0, 3) - B(2, 1) + B(2, 3);

    D[4] = B(1, 0) - B(1, 2) + B(2, 0) - B(2, 2);
    D[5] = B(1, 1) + B(1, 2) + B(2, 1) + B(2, 2);
    D[6] = -B(1, 1) + B(1, 2) - B(2, 1) + B(2, 2);
    D[7] = B(1, 1) - B(1, 3) + B(2, 1) - B(2, 3);

    D[8] = -B(1, 0) + B(1, 2) + B(2, 0) - B(2, 2);
    D[9] = -B(1, 1) - B(1, 2) + B(2, 1) + B(2, 2);
    D[10] = B(1, 1) - B(1, 2) - B(2, 1) + B(2, 2);
    D[11] = -B(1, 1) + B(1, 3) + B(2, 1) - B(2, 3);

    D[12] = B(1, 0) - B(1, 2) - B(3, 0) + B(3, 2);
    D[13] = B(1, 1) + B(1, 2) - B(3, 1) - B(3, 2);
    D[14] = -B(1, 1) + B(1, 2) + B(3, 1) - B(3, 2);
    D[15] = B(1, 1) - B(1, 3) - B(3, 1) + B(3, 3);

#undef B
}
