#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include <fixedpointvariables.h>

class MaxPooling
{
public:
    MaxPooling(unsigned int);

    layerOut_t *apply(layerOut_t *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC);

private:
    const unsigned int stride;
};

#endif // MAXPOOLING_H
