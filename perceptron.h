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

    Perceptron(const std::string name);

    void apply(ac_channel<memInStruct> &I, ac_channel<memOutStruct> &Y,
               const perceptronKernel_t K[sizeKx * sizeKy], const perceptronBias_t B[sizeKy]);


private:
    const std::string name;
};

///
/// IMPLEMENTATION
///

template<unsigned int sizeKx, unsigned int sizeKy>
Perceptron<sizeKx, sizeKy>::
Perceptron(const std::string name) :
    name(name)
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
apply(ac_channel<memInStruct> &I, ac_channel<memOutStruct> &Y, const perceptronKernel_t K[sizeKx * sizeKy],
      const perceptronBias_t B[sizeKy])
{
    memInStruct  bufferI = I.read();
    memOutStruct bufferY;

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
