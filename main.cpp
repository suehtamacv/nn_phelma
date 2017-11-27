#ifdef __SIMULATION__

#include <iostream>

#include "maxpooling.h"
#include "softmax.h"
#include "png_utils.h"
#include "relu.h"
#include "matrixmultiply.h"

#endif

#include "convolution.h"
#include "fixedpointvariables.h"

#define WIDTH 256
#define HEIGHT 256

static layerOut_t ImageIn[HEIGHT * WIDTH * 3] = {0};
static layerOut_t ImageOut[HEIGHT * WIDTH * 3] = {0};

static const convKernel_t KernelImp[3 * 3 * 3 * 3] = {0, 0, 0, 0, 1, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 1, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 1, 0, 0, 0, 0
                                                     };
static const convKernel_t Gaussian[3 * 3 * 3 * 3] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16
                                                    };
static Convolution<HEIGHT, WIDTH, 3, 3> C(KernelImp);

#pragma design top
void apply(layerOut_t* In, layerOut_t* Out)
{
    Out = C.apply(In);
}

#ifdef __SIMULATION__

int main()
{
    // Reads PNG
    readPNG("lena.png");
    flattenPNG(ImageIn);

    apply(ImageIn, ImageOut);

    unflattenPNG(ImageOut);
    writePNG("lena2.png");

    return 0;
}

#endif
