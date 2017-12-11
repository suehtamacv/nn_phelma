#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define overlap  2
#define tileSize 4

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
class ConvolutionReLU
{
public:
    typedef memInterface<sizeY * sizeX * sizeC> memInStruct;
    typedef memInterface<sizeY * sizeX * sizeL> memOutStruct;

    ConvolutionReLU(const std::string name,
                    const convKernel_t (&K)[sizeC * sizeL * 3 * 3], const convBias_t (&B)[sizeL],
                    ac_channel<memOutStruct> &Y);

    void apply(ac_channel<memInStruct> &I);

    ac_channel<memOutStruct> &Y;

private:
    ///
    /// \brief K is the convolution kernel.
    /// Size : sizeC * sizeL * 3 * 3
    ///
    const convKernel_t K[sizeC * sizeL * 3 * 3];

    ///
    /// \brief B are the biases.
    /// Size : sizeL
    ///
    const convBias_t B[sizeL];

    memInStruct  bufferI;
    memOutStruct bufferY;

    const std::string name;

    void calculateG(convKernel_t (&G)[16], const convKernel_t *);
    void calculateD(layerOut_t (&Block)[16], convD_t (&D)[16]);
    void getImageBlock(memInStruct &, layerOut_t *Block, const unsigned int xI, const unsigned int yI,
                       const unsigned int cI);
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
ConvolutionReLU<sizeX, sizeY, sizeC, sizeL>::
ConvolutionReLU(const std::string name,
                const convKernel_t (&K)[sizeC * sizeL * 3 * 3], const convBias_t (&B)[sizeL],
                ac_channel<memOutStruct> &Y) :
    Y(Y), K(K), B(B), name(name)
{
#ifdef __STAT__
    for (unsigned int i = 0; i < sizeC * sizeL * 3 * 3; ++i)
        {
        std::cout << name << " K " << K[i] << std::endl;
        }

    for (unsigned int i = 0; i < sizeL; ++i)
        {
        std::cout << name << " B " << B[i] << std::endl;
        }
#endif

}

#pragma design
template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void ConvolutionReLU<sizeX, sizeY, sizeC, sizeL>::apply(ac_channel<memInStruct> &I)
{
    if (!I.available(1))
        {
        return;
        }

    bufferI = I.read();

    convKernel_t G[tileSize * tileSize];
    convD_t D[tileSize * tileSize];
    convM_t M[tileSize * tileSize];
    convTemp_t temp[2][4];
    layerOut_t Block[tileSize * tileSize];
    layerOut_t preReLU[2][2];

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
                    getImageBlock(bufferI, Block, xI, yI, cI);

                    // Sends pointer to K(:, :, l, c) at (l * sizeC + c) * 3 * 3
                    calculateG(G, K + ((lI * sizeC + cI) * 9));
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
                    preReLU[j][0] = temp[j][0] + temp[j][1] + temp[j][2] + B[lI];
                    preReLU[j][1] = temp[j][1] - temp[j][2] - temp[j][3] + B[lI];

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
}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void ConvolutionReLU<sizeX, sizeY, sizeC, sizeL>::
calculateG(convKernel_t (&G)[16], const convKernel_t *K)
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

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void ConvolutionReLU<sizeX, sizeY, sizeC, sizeL>::
calculateD(layerOut_t (&Block)[16], convD_t (&D)[16])
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

#ifdef __STAT__
    for (unsigned int i = 0; i < 16; ++i)
        {
        std::cout << name << " D " << D[i] << std::endl;
        }
#endif

#undef B
}

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
void ConvolutionReLU<sizeX, sizeY, sizeC, sizeL>::
getImageBlock(memInStruct &I, layerOut_t *Block, const unsigned int xI, const unsigned int yI,
              const unsigned int cI)
{
#define B(y, x) \
    Block[y * tileSize + x]

#ifdef __HWC__
#define T(y, x) \
    I[(yI + (y)) * sizeX * sizeC + (xI + (x)) * sizeC + cI]
#else
#define T(y, x) \
    I.Y[cI * sizeY * sizeX + (yI + (y)) * sizeX + (xI + (x))]
#endif


    const bool xBorderL = (xI == 0);
    const bool xBorderR = (xI + tileSize > sizeX);
    const bool yBorderT = (yI == 0);
    const bool yBorderB = (yI + tileSize > sizeY);

    const unsigned int yLimInf = yBorderT ? 1 : 0;
    const unsigned int yLimSup = yBorderB ? 2 : 3;
    const unsigned int xLimInf = xBorderL ? 1 : 0;
    const unsigned int xLimSup = xBorderR ? 2 : 3;

loopImageBlockY:
    for (unsigned int j = 0; j < 4; ++j)
        {
loopImageBlockX:
        for (unsigned int i = 0; i < 4; ++i)
            {
            B(j, i) = ((j >= yLimInf) && (j <= yLimSup) && (i >= xLimInf) && (i <= xLimSup)) ? T(j - 1, i - 1) : 0;
            }
        }

#ifdef __STAT__
    for (unsigned int i = 0; i < 16; ++i)
        {
        std::cout << name << " B " << Block[i] << std::endl;
        }
#endif

#undef T
#undef B
}

#endif // CONVOLUTION_H
