#ifndef FIXEDPOINTVARIABLES_H
#define FIXEDPOINTVARIABLES_H

#include <ac_fixed.h>

#ifdef __SIMULATION__

#define INPUT_BITS_PER_PIXEL 8

#define LAYER_OUTPUT_DYNAMIC   9 // -255 to 255
#define LAYER_OUTPUT_PRECISION 3

#define KERNEL_DYNAMIC   8 // -127 to 127
#define KERNEL_PRECISION 4

#endif

typedef ac_int<INPUT_BITS_PER_PIXEL, false> img_t;
typedef ac_fixed < LAYER_OUTPUT_DYNAMIC + LAYER_OUTPUT_PRECISION, LAYER_OUTPUT_PRECISION, false > layerOut_t;
typedef ac_fixed < KERNEL_DYNAMIC + KERNEL_PRECISION, KERNEL_DYNAMIC, true > kernel_t;

// Convolution types
typedef ac_fixed < LAYER_OUTPUT_DYNAMIC + LAYER_OUTPUT_PRECISION + 2, LAYER_OUTPUT_PRECISION, false > convD_t;
typedef ac_fixed < LAYER_OUTPUT_DYNAMIC + LAYER_OUTPUT_PRECISION + 2, LAYER_OUTPUT_PRECISION, false > convR_t;

#endif // FIXEDPOINTVARIABLES_H
