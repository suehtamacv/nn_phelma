#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <fixedpointvariables.h>

class Convolution
{
public:
    Convolution(const kernel_t *K,
                unsigned int kernelSizeC,
                unsigned int kernelSizeL);

    layerOut_t *apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY);

private:
    ///
    /// \brief K is the convolution kernel.
    ///
    /// It has size kernelSizeC x kernelSizeL x 3 x 3.
    ///
    const kernel_t *K;
    unsigned int kernelSizeC, kernelSizeL;

    void calculateG(kernel_t *, const unsigned int);
    void calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
                    const unsigned int yI, const unsigned int cI,
                    const unsigned int sizeX, const unsigned int sizeY,
                    const unsigned int sizeC);
};

#endif // CONVOLUTION_H
