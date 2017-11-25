#ifndef MATRIXMULTIPLY_H
#define MATRIXMULTIPLY_H

#include <fixedpointvariables.h>

template<unsigned int sizeKx, unsigned int sizeKy, unsigned int sizeY>
class MatrixMultiply
{
public:
    MatrixMultiply(const matrixProdKernel_t K[sizeKx * sizeKy]);

    void apply(layerOut_t*);

    layerOut_t Y[sizeKx * sizeY];

private:
    const matrixProdKernel_t K[sizeKx * sizeKy];
};

#include "matrixmultiply.tpp"

#endif // MATRIXMULTIPLY_H
