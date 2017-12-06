#ifndef NNARRAYS_H
#define NNARRAYS_H

#include "fixedpointvariables.h"

#define WIDTH  24
#define HEIGHT 24

#define INPUT_SIZE    HEIGHT * WIDTH * 3
#define OUTPUT_SIZE   HEIGHT * WIDTH * 64

static layerOut_t ImageIn[INPUT_SIZE] = {0};
static layerOut_t ImageOut[OUTPUT_SIZE] = {0};

#include "biases.h"
#include "kernels.h"

#endif // NNARRAYS_H
