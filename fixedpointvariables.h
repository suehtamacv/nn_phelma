#ifndef FIXEDPOINTVARIABLES_H
#define FIXEDPOINTVARIABLES_H

#include <ac_fixed.h>

#ifdef __SIMULATION__

#define LAYER_OUTPUT_DYN   32
#define LAYER_OUTPUT_PREC  32

#define CONV_KERNEL_DYN    32
#define CONV_KERNEL_PREC   32

#define CONV_BIAS_DYN      32
#define CONV_BIAS_PREC     32

#define CONV_M_DYN         32
#define CONV_M_PREC        32

#define CONV_D_DYN         32
#define CONV_D_PREC        32

#define PERCEP_KERNEL_DYN  32
#define PERCEP_KERNEL_PREC 32

#define PERCEP_BIAS_DYN    32
#define PERCEP_BIAS_PREC   32

#else

#define LAYER_OUTPUT_DYN   8 // -128 to 127
#define LAYER_OUTPUT_PREC  4

#define CONV_KERNEL_DYN    3 // -4 to 3
#define CONV_KERNEL_PREC   9

#define CONV_BIAS_DYN      3 // -4 to 3
#define CONV_BIAS_PREC     5

#define CONV_M_DYN         7 // -64 to 63
#define CONV_M_PREC        6

#define CONV_D_DYN         7 // -64 to 63
#define CONV_D_PREC        3

#define CONV_TEMP_DYN      7 // -64 to 63
#define CONV_TEMP_PREC     3

#define PERCEP_KERNEL_DYN  0 // -.5 to .5
#define PERCEP_KERNEL_PREC 4

#define PERCEP_BIAS_DYN    3 // -4 to 3
#define PERCEP_BIAS_PREC   1

#endif

#ifdef __FLOATVERSION__

// Layer output
typedef double layerOut_t;

// Convolution types
typedef double convD_t;
typedef double convM_t;
typedef double convTemp_t;
typedef double convKernel_t;
typedef double convBias_t;

// Matrix product types
typedef double perceptronKernel_t;
typedef double perceptronBias_t;

#else

// Layer output
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC, LAYER_OUTPUT_DYN, true > layerOut_t;

// Convolution types
typedef ac_fixed < CONV_D_DYN + CONV_D_PREC, CONV_D_DYN, true > convD_t;
typedef ac_fixed < CONV_M_DYN + CONV_M_PREC, CONV_M_DYN, true > convM_t;
typedef ac_fixed < CONV_TEMP_DYN + CONV_TEMP_PREC, CONV_TEMP_DYN, true > convTemp_t;
typedef ac_fixed < CONV_KERNEL_DYN + CONV_KERNEL_PREC, CONV_KERNEL_DYN, true > convKernel_t;
typedef ac_fixed < CONV_BIAS_DYN + CONV_BIAS_PREC, CONV_BIAS_DYN, true > convBias_t;

// Matrix product types
typedef ac_fixed < PERCEP_KERNEL_DYN + PERCEP_KERNEL_PREC, PERCEP_KERNEL_DYN, true > perceptronKernel_t;
typedef ac_fixed < PERCEP_BIAS_DYN + PERCEP_BIAS_PREC, PERCEP_BIAS_DYN, true > perceptronBias_t;

#endif

#endif // FIXEDPOINTVARIABLES_H
