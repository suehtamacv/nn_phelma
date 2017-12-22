#include "convolutionrelu.h"
#include "kernels.h"
#include "biases.h"

#pragma design
void conv1_apply(ac_channel<conv1_In_t> &I, ac_channel<conv1_Out_t> &Y)
{
#define sizeX 24
#define sizeY 24
#define sizeC 3
#define sizeL 64

    conv1_In_t  bufferI = I.read();
    conv1_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

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
            for (unsigned int lI = 0; lI < sizeL; lI++)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeY, sizeC, sizeL>(bufferI, Block, xI, yI, cI);

                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    calculateG(G, convKernel1 + ((lI * sizeC + cI) * 9));
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
#ifdef __STAT__
                        std::cout << name << " M " << M[i] << std::endl;
#endif
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];

#ifdef __STAT__
                    std::cout << name << " temp " << temp[0][i] << std::endl;
                    std::cout << name << " temp " << temp[1][i] << std::endl;
#endif
                    }

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias1[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias1[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

#ifdef __STAT__
                    std::cout << name << " preReLU " << preReLU[j][0] << std::endl;
                    std::cout << name << " preReLU " << preReLU[j][1] << std::endl;
#endif

#ifdef __HWC__
                    bufferY.Y[yI * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][0];
                    bufferY.Y[(yI + 1) * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][1];
#else
                    bufferY.Y[lI * sizeY * sizeX + yI * sizeX + (xI + j)] = preReLU[j][0];
                    bufferY.Y[lI * sizeY * sizeX + (yI + 1) * sizeX + (xI + j)] = preReLU[j][1];
#endif
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

#pragma design
void conv2_apply(ac_channel<conv2_In_t> &I, ac_channel<conv2_Out_t> &Y)
{
#define sizeX 12
#define sizeY 12
#define sizeC 64
#define sizeL 32

    conv2_In_t  bufferI = I.read();
    conv2_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

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
            for (unsigned int lI = 0; lI < sizeL; lI++)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeY, sizeC, sizeL>(bufferI, Block, xI, yI, cI);

                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    calculateG(G, convKernel2 + ((lI * sizeC + cI) * 9));
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
#ifdef __STAT__
                        std::cout << name << " M " << M[i] << std::endl;
#endif
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];

#ifdef __STAT__
                    std::cout << name << " temp " << temp[0][i] << std::endl;
                    std::cout << name << " temp " << temp[1][i] << std::endl;
#endif
                    }

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias2[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias2[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

#ifdef __STAT__
                    std::cout << name << " preReLU " << preReLU[j][0] << std::endl;
                    std::cout << name << " preReLU " << preReLU[j][1] << std::endl;
#endif

#ifdef __HWC__
                    bufferY.Y[yI * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][0];
                    bufferY.Y[(yI + 1) * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][1];
#else
                    bufferY.Y[lI * sizeY * sizeX + yI * sizeX + (xI + j)] = preReLU[j][0];
                    bufferY.Y[lI * sizeY * sizeX + (yI + 1) * sizeX + (xI + j)] = preReLU[j][1];
#endif
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

#pragma design
void conv3_apply(ac_channel<conv3_In_t> &I, ac_channel<conv3_Out_t> &Y)
{
#define sizeX 6
#define sizeY 6
#define sizeC 32
#define sizeL 20

    conv3_In_t  bufferI = I.read();
    conv3_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

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
            for (unsigned int lI = 0; lI < sizeL; lI++)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeY, sizeC, sizeL>(bufferI, Block, xI, yI, cI);

                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    calculateG(G, convKernel3 + ((lI * sizeC + cI) * 9));
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
#ifdef __STAT__
                        std::cout << name << " M " << M[i] << std::endl;
#endif
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];

#ifdef __STAT__
                    std::cout << name << " temp " << temp[0][i] << std::endl;
                    std::cout << name << " temp " << temp[1][i] << std::endl;
#endif
                    }

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias3[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias3[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

#ifdef __STAT__
                    std::cout << name << " preReLU " << preReLU[j][0] << std::endl;
                    std::cout << name << " preReLU " << preReLU[j][1] << std::endl;
#endif

#ifdef __HWC__
                    bufferY.Y[yI * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][0];
                    bufferY.Y[(yI + 1) * sizeX * sizeL + (xI + j) * sizeL + lI] = preReLU[j][1];
#else
                    bufferY.Y[lI * sizeY * sizeX + yI * sizeX + (xI + j)] = preReLU[j][0];
                    bufferY.Y[lI * sizeY * sizeX + (yI + 1) * sizeX + (xI + j)] = preReLU[j][1];
#endif
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

#ifdef __STAT__
    for (unsigned int i = 0; i < 16; ++i)
        {
        std::cout << name << " G " << G[i] << std::endl;
        }
#endif

#undef K
}

void calculateD(pixel_t (&Block)[16], convD_t (&D)[16])
{
#define B(x, y) \
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

#ifdef __STAT__
    for (unsigned int i = 0; i < 16; ++i)
        {
        std::cout << name << " D " << D[i] << std::endl;
        }
#endif

#undef B
}
