#ifndef FIXEDPOINTVARIABLES_H
#define FIXEDPOINTVARIABLES_H

#include <ac_fixed.h>

#ifdef __SIMULATION__

#define INPUT_BITS_PER_PIXEL 8

#define LAYER_OUTPUT_DYN   9 // -255 to 255
#define LAYER_OUTPUT_PREC 3

#define KERNEL_DYN   8 // -127 to 127
#define KERNEL_PREC 4

#endif

typedef ac_int<INPUT_BITS_PER_PIXEL, false> img_t;
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC, LAYER_OUTPUT_DYN, true > layerOut_t;
typedef ac_fixed < KERNEL_DYN + KERNEL_PREC, KERNEL_DYN, true > kernel_t;

// Convolution types
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC + 2, LAYER_OUTPUT_DYN, true > convD_t;
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC - 1, LAYER_OUTPUT_DYN, true > convM_t;
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC, LAYER_OUTPUT_DYN, true > convTemp_t;

#endif // FIXEDPOINTVARIABLES_H
