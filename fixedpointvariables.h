#ifndef FIXEDPOINTVARIABLES_H
#define FIXEDPOINTVARIABLES_H

#include <ac_fixed.h>

#ifdef __SIMULATION__

#define INPUT_BITS_PER_PIXEL 8

#define LAYER_OUTPUT_DYN   9 // -255 to 255
#define LAYER_OUTPUT_PREC  7

#define KERNEL_DYN   8 // -127 to 127
#define KERNEL_PREC  4

#define BIAS_DYN   8 // -127 to 127
#define BIAS_PREC  4

#define SOFTMAX_DYN  8  // 0 to 255
#define SOFTMAX_PREC 8

#define MATRIX_PROD_DYN  KERNEL_DYN
#define MATRIX_PROD_PREC KERNEL_PREC

#else

#define INPUT_BITS_PER_PIXEL 8

#define LAYER_OUTPUT_DYN   9 // -255 to 255
#define LAYER_OUTPUT_PREC  7

#define KERNEL_DYN   8 // -127 to 127
#define KERNEL_PREC  4

#define SOFTMAX_DYN  8  // 0 to 255
#define SOFTMAX_PREC 8

#define BIAS_DYN   8 // -127 to 127
#define BIAS_PREC  4

#define MATRIX_PROD_DYN  KERNEL_DYN
#define MATRIX_PROD_PREC KERNEL_PREC

#endif

// Layer output
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC, LAYER_OUTPUT_DYN, true > layerOut_t;

// Convolution types
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC + 2, LAYER_OUTPUT_DYN + 2, true > convD_t;
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC - 1, LAYER_OUTPUT_DYN - 1, true > convM_t;
typedef ac_fixed < LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC, LAYER_OUTPUT_DYN, true > convTemp_t;
typedef ac_fixed < KERNEL_DYN + KERNEL_PREC, KERNEL_DYN, true > convKernel_t;
typedef ac_fixed < BIAS_DYN + BIAS_PREC, BIAS_DYN, true > convBias_t;

// Softmax types
typedef ac_fixed < SOFTMAX_PREC, 2, false > softE_t;
typedef ac_fixed < SOFTMAX_DYN + SOFTMAX_PREC, SOFTMAX_DYN, false > softSE_t;

// Matrix product types
typedef ac_fixed < MATRIX_PROD_DYN + MATRIX_PROD_PREC, MATRIX_PROD_DYN, true > matrixProdKernel_t;

#endif // FIXEDPOINTVARIABLES_H
