#ifndef SOFTMAX_H
#define SOFTMAX_H

#include <ac_fixed.h>

class Softmax
{
public:
    Softmax();
    uint8* apply(int8 *I, const unsigned int sizeX);

private:
    int8 expFP(const int8 &x);
};

#endif // SOFTMAX_H
