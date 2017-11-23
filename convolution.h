#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <ac_fixed.h>

class Convolution
{
public:
    Convolution(const int *K,
                unsigned int kernelSizeC,
                unsigned int kernelSizeL);

    uint8* apply(uint8 *I, unsigned int sizeX, unsigned int sizeY);

private:
    ///
    /// \brief K is the convolution kernel.
    ///
    /// It has size kernelSizeC x kernelSizeL x 3 x 3.
    ///
    const int *K;
    unsigned int kernelSizeC, kernelSizeL;

    void calculateG(int *, const unsigned int);
    void calculateD(uint8 *I, int *D, const unsigned int xI,
                    const unsigned int yI, const unsigned int cI,
                    const unsigned int sizeX, const unsigned int sizeY,
                    const unsigned int sizeC);
};

#endif // CONVOLUTION_H
