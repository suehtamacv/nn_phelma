#ifdef __SIMULATION__

#include <iostream>

#include "maxpooling.h"
#include "softmax.h"
#include "png_utils.h"
#include "matrixmultiply.h"
#include "relu.h"

#endif

#include "convolutionrelu.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

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
static const convBias_t Biases[3] = {0, 0, 0};

#pragma hls_design top
void apply(layerOut_t In[INPUT_SIZE], layerOut_t Out[OUTPUT_SIZE])
{
    ConvolutionReLU<HEIGHT, WIDTH, 3, 64> Conv1(KernelImp, Biases, Out);
    Conv1.apply(In);
}

int main()
{
    // Reads PNG
#ifdef __SIMULATION__
    readPNG("lena.png");
    flattenPNG(ImageIn);
#endif

    apply(ImageIn, ImageOut);

#ifdef __SIMULATION__
    unflattenPNG(ImageOut);
    writePNG("out.png");
#else
    for (unsigned int i = 0; i < OUTPUT_SIZE; ++i)
        {
        std::cout << "(" << i << ")\t" << (int8) (ImageOut[i]).slc<INPUT_BITS_PER_PIXEL>
                  (LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC - 1 - INPUT_BITS_PER_PIXEL) << std::endl;
        }
#endif

    return 0;
}
