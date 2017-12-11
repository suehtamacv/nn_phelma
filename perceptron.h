#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "fixedpointvariables.h"
#include "meminterface.h"

template<unsigned int sizeKx, unsigned int sizeKy>
class Perceptron
{
public:
    typedef memInterface<sizeKx> memInStruct;
    typedef memInterface<sizeKy> memOutStruct;

    Perceptron(const std::string name, const perceptronKernel_t (&K)[sizeKx * sizeKy],
               const perceptronBias_t (&B)[sizeKy],
               ac_channel<memOutStruct> &Y);

    void apply(ac_channel<memInStruct>&);

    ///
    /// \brief Y
    /// Size : sizeKy
    ///
    ac_channel<memOutStruct> &Y;

private:
    memInStruct  bufferI;
    memOutStruct bufferY;

    ///
    /// \brief K
    /// Size : sizeKy * sizeKx
    ///
    const perceptronKernel_t K[sizeKx * sizeKy];

    ///
    /// \brief B
    /// Size : sizeKy
    const perceptronBias_t B[sizeKy];

    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy>
Perceptron<sizeKx, sizeKy>::
Perceptron(const std::string name, const perceptronKernel_t (&K)[sizeKx * sizeKy], const perceptronBias_t (&B)[sizeKy],
           ac_channel<memOutStruct> &Y) : Y(Y),
    K(K), B(B), name(name)
{
#ifdef __STAT__
    for (unsigned int i = 0; i < sizeKy * sizeKx; ++i)
        {
        std::cout << name << " K " << K[i] << std::endl;
        }

    for (unsigned int i = 0; i < sizeKx; ++i)
        {
        std::cout << name << " B " << B[i] << std::endl;
        }
#endif
}

#pragma design
template<unsigned int sizeKx, unsigned int sizeKy>
void Perceptron<sizeKx, sizeKy>::
apply(ac_channel<memInStruct> &I)
{
    if (!I.available(1))
        {
        return;
        }
    bufferI = I.read();

loopY:
    for (unsigned int iKy = 0; iKy < sizeKy; ++iKy)
        {
        bufferY.Y[iKy] = B[iKy];

loopX:
        for (unsigned int iKx = 0; iKx < sizeKx; ++iKx)
            {
            bufferY.Y[iKy] += K[iKy * sizeKx + iKx] * bufferI.Y[iKx];
            }

#ifdef __STAT__
        std::cout << name << " Y " << Y[iKy] << std::endl;
#endif
        }

    Y.write(bufferY);

}

#endif // PERCEPTRON_H
