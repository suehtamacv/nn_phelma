#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include <ac_fixed.h>

class MaxPooling
{
public:
    MaxPooling(unsigned int);

    uint8 *apply(uint8 *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC);

private:
    const unsigned int stride;
};

#endif // MAXPOOLING_H
