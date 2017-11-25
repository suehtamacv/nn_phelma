#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <fixedpointvariables.h>

template<unsigned int sizeX, unsigned int sizeY, unsigned int sizeC, unsigned int sizeL>
class Convolution
{
public:
    Convolution(const convKernel_t K[sizeC * sizeL * 3 * 3]);

    void apply(layerOut_t *I);

    ///
    /// \brief Y is the output matrix.
    ///
    layerOut_t Y[sizeY * sizeX * sizeL];

private:
    ///
    /// \brief K is the convolution kernel.
    ///
    convKernel_t K[sizeC * sizeL * 3 * 3];

    void calculateG(convKernel_t *, const unsigned int);
    void calculateD(layerOut_t *I, convD_t *D, const unsigned int xI,
                    const unsigned int yI, const unsigned int cI);
};

#include "convolution.tpp"

#endif // CONVOLUTION_H
