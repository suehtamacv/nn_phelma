#ifndef NNARRAYS_H
#define NNARRAYS_H

#include "fixedpointvariables.h"

#include "image1.h"

#ifdef __SIMULATION__

#define WIDTH  256
#define HEIGHT 256

#define INPUT_SIZE    HEIGHT * WIDTH * 3
#define OUTPUT_SIZE   HEIGHT * WIDTH * 3

static layerOut_t ImageIn[INPUT_SIZE] = {0};
static layerOut_t ImageOut[OUTPUT_SIZE] = {0};

#else

#define WIDTH  32
#define HEIGHT 32

#define INPUT_SIZE    HEIGHT * WIDTH * 3
#define OUTPUT_SIZE   HEIGHT * WIDTH * 3

static layerOut_t *ImageIn = (layerOut_t*) Image1;
static layerOut_t ImageOut[OUTPUT_SIZE] = {0};

#endif



#endif // NNARRAYS_H