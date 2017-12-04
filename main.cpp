#ifdef __SIMULATION__

#include <iostream>

#include "softmax.h"
#include "png_utils.h"

#endif

#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
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

void applyComplete(layerOut_t In[24 * 24 * 3], layerOut_t Out[10])
{
    layerOut_t Conv1_Out[24 * 24 * 64];
    layerOut_t MaxPool1_Out[12 * 12 * 64];
    layerOut_t Conv2_Out[12 * 12 * 32];
    layerOut_t MaxPool2_Out[6 * 6 * 32];
    layerOut_t Conv3_Out[6 * 6 * 20];
    layerOut_t MaxPool3_Out[3 * 3 * 20];

    ConvolutionReLU<24, 24, 3, 64> Conv1(KernelImp, Biases, Conv1_Out);
    MaxPooling<2, 24, 24, 64> MaxPool1(MaxPool1_Out);
    ConvolutionReLU<12, 12, 64, 32> Conv2(KernelImp, Biases, Conv2_Out);
    MaxPooling<2, 12, 12, 32> MaxPool2(MaxPool2_Out);
    ConvolutionReLU<6, 6, 32, 20> Conv3(KernelImp, Biases, Conv3_Out);
    MaxPooling<2, 6, 6, 20> MaxPool3(MaxPool3_Out);
    Perceptron<180, 10, 1> Percep4(KernelImp, Out);

    Conv1.apply(In);
    MaxPool1.apply(Conv1.Y);
    Conv2.apply(MaxPool1.Y);
    MaxPool2.apply(Conv2.Y);
    Conv3.apply(MaxPool2.Y);
    MaxPool3.apply(Conv3.Y);
    Percep4.apply(MaxPool3.Y);
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
