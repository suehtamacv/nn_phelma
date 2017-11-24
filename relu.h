//
//  relu.h
//
//
//  Created by BOUTOUR Azza on 24/11/2017.
//
//

#ifndef RELU_H
#define RELU_H

#include "fixedpointvariables.h"

class ReLU
{
public:
    ReLU();
    uint8 *apply(int8 *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC);

private:
    inline uint8 max(int8 x);
};

#endif // RELU_H


