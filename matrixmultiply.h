#ifndef MATRIXMULTIPLY_H
#define MATRIXMULTIPLY_H

#include <ac_fixed.h>

class MatrixMultiply
{
public:
    MatrixMultiply(const uint8 *K,
                   const unsigned int sizeKx,
                   const unsigned int sizeKy);

    uint8* apply(uint8*, const unsigned int sizeY);

private:
    const uint8 *K;
    const unsigned int sizeKx;
    const unsigned int sizeKy;

};

#endif // MATRIXMULTIPLY_H
