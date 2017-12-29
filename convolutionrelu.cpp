#include "convolutionrelu.h"
#include "kernels.h"
#include "biases.h"

#pragma design
void conv1_apply(ac_channel<conv1_line_In_t> &I, ac_channel<conv1_line_Out_t> &Y)
{
#define sizeX 24
#define sizeY 24
#define sizeC 3
#define sizeL 64

    conv1_line_In_t  bufferI_New;
    conv1_line_In_t  bufferI_Old;
    conv1_line_In_t  bufferI_Zero;
    conv1_line_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

    // Y coordinate
loopConvYi:
    for (unsigned int yI = 0; yI < sizeY; yI += overlap)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - overlap)
            {
            bufferI_New = bufferI_Zero;
            }
        else
            {
            bufferI_New = I.read();
            }

        // Output channels
loopConvOutChannel:
        for (unsigned int lI = 0; lI < sizeL; lI++)
            {

            // X coordinate
loopConvXi:
            for (unsigned int xI = 0; xI < sizeX; xI += overlap)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeC>(bufferI_Old, bufferI_New, Block, xI, cI);

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
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                    }

                layerOutBlock_t outBlock;
                pixel_t maxBlock;
                maxBlock.set_val<AC_VAL_MIN>();

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias1[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias1[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

                    outBlock.P[2 * j] = preReLU[j][0];
                    outBlock.P[2 * j + 1] = preReLU[j][1];

                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j];
                        outBlock.biggerBlock = 2 * j;
                        }
                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j + 1];
                        outBlock.biggerBlock = 2 * j + 1;
                        }
                    }

                if (outBlock.P[0] > outBlock.P[1])
                    {
                    outBlock.biggerH = 0;
                    }
                else
                    {
                    outBlock.biggerH = 1;
                    }

                if (outBlock.P[0] > outBlock.P[2])
                    {
                    outBlock.biggerV = 0;
                    }
                else
                    {
                    outBlock.biggerV = 1;
                    }

                bufferY.Y[lI * (sizeX / BLOCK_HEIGHT) + (xI / BLOCK_HEIGHT)] = outBlock;
                // End transformation
                }
            }
        Y.write(bufferY);
        }

#undef sizeX
#undef sizeY
#undef sizeC
#undef sizeL
}

#pragma design
void conv2_apply(ac_channel<conv2_line_In_t> &I, ac_channel<conv2_line_Out_t> &Y)
{
#define sizeX 12
#define sizeY 12
#define sizeC 64
#define sizeL 32

    conv2_line_In_t  bufferI_New;
    conv2_line_In_t  bufferI_Old;
    conv2_line_In_t  bufferI_Zero;
    conv2_line_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

    // Y coordinate
loopConvYi:
    for (unsigned int yI = 0; yI < sizeY; yI += overlap)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - overlap)
            {
            bufferI_New = bufferI_Zero;
            }
        else
            {
            bufferI_New = I.read();
            }

        // Output channels
loopConvOutChannel:
        for (unsigned int lI = 0; lI < sizeL; lI++)
            {

            // X coordinate
loopConvXi:
            for (unsigned int xI = 0; xI < sizeX; xI += overlap)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeC>(bufferI_Old, bufferI_New, Block, xI, cI);

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
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                    }

                layerOutBlock_t outBlock;
                pixel_t maxBlock;
                maxBlock.set_val<AC_VAL_MIN>();

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias1[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias1[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

                    outBlock.P[2 * j] = preReLU[j][0];
                    outBlock.P[2 * j + 1] = preReLU[j][1];

                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j];
                        outBlock.biggerBlock = 2 * j;
                        }
                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j + 1];
                        outBlock.biggerBlock = 2 * j + 1;
                        }
                    }

                if (outBlock.P[0] > outBlock.P[1])
                    {
                    outBlock.biggerH = 0;
                    }
                else
                    {
                    outBlock.biggerH = 1;
                    }

                if (outBlock.P[0] > outBlock.P[2])
                    {
                    outBlock.biggerV = 0;
                    }
                else
                    {
                    outBlock.biggerV = 1;
                    }

                bufferY.Y[lI * (sizeX / BLOCK_HEIGHT) + (xI / BLOCK_HEIGHT)] = outBlock;
                // End transformation
                }
            }
        Y.write(bufferY);
        }

#undef sizeX
#undef sizeY
#undef sizeC
#undef sizeL
}

#pragma design
void conv3_apply(ac_channel<conv3_line_In_t> &I, ac_channel<conv3_line_Out_t> &Y)
{
#define sizeX 6
#define sizeY 6
#define sizeC 32
#define sizeL 20

    conv3_line_In_t  bufferI_New;
    conv3_line_In_t  bufferI_Old;
    conv3_line_In_t  bufferI_Zero;
    conv3_line_Out_t bufferY;

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    pixel_t Block[tileSize * tileSize];
    pixel_t preReLU[2][2];

    // Y coordinate
loopConvYi:
    for (unsigned int yI = 0; yI < sizeY; yI += overlap)
        {
        bufferI_Old = bufferI_New;
        if (yI == sizeY - overlap)
            {
            bufferI_New = bufferI_Zero;
            }
        else
            {
            bufferI_New = I.read();
            }

        // Output channels
loopConvOutChannel:
        for (unsigned int lI = 0; lI < sizeL; lI++)
            {

            // X coordinate
loopConvXi:
            for (unsigned int xI = 0; xI < sizeX; xI += overlap)
                {

                // Input channels
loopConvInChannel:
                for (unsigned int cI = 0; cI < sizeC; cI++)
                    {
                    getImageBlock<sizeX, sizeC>(bufferI_Old, bufferI_New, Block, xI, cI);

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
                        }
                    }

                // Transform to image space
loopInverseTransform:
                for (unsigned int i = 0; i < 4; ++i)
                    {
                    temp[0][i] = M[tileSize * i] + M[tileSize * i + 1] + M[tileSize * i + 2];
                    temp[1][i] = M[tileSize * i + 1] - M[tileSize * i + 2] - M[tileSize * i + 3];
                    }

                layerOutBlock_t outBlock;
                pixel_t maxBlock;
                maxBlock.set_val<AC_VAL_MIN>();

loopOutputBlock:
                for (unsigned int j = 0; j < 2; ++j)
                    {
                    // Inverse transform
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + convBias1[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + convBias1[lI];

                    // Applies ReLU
                    preReLU[j][0] = (preReLU[j][0] >= 0) ? preReLU[j][0] : 0;
                    preReLU[j][1] = (preReLU[j][1] >= 0) ? preReLU[j][1] : 0;

                    outBlock.P[2 * j] = preReLU[j][0];
                    outBlock.P[2 * j + 1] = preReLU[j][1];

                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j];
                        outBlock.biggerBlock = 2 * j;
                        }
                    if (maxBlock < outBlock.P[2 * j])
                        {
                        maxBlock = outBlock.P[2 * j + 1];
                        outBlock.biggerBlock = 2 * j + 1;
                        }
                    }

                if (outBlock.P[0] > outBlock.P[1])
                    {
                    outBlock.biggerH = 0;
                    }
                else
                    {
                    outBlock.biggerH = 1;
                    }

                if (outBlock.P[0] > outBlock.P[2])
                    {
                    outBlock.biggerV = 0;
                    }
                else
                    {
                    outBlock.biggerV = 1;
                    }

                bufferY.Y[lI * (sizeX / BLOCK_HEIGHT) + (xI / BLOCK_HEIGHT)] = outBlock;
                // End transformation
                }
            }
        Y.write(bufferY);
        }

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
