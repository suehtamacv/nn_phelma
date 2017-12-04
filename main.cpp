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

    ConvolutionReLU<24, 24, 3, 64> Conv1(convKernel1, convBias1, Conv1_Out);
    MaxPooling<2, 3, 24, 24, 64> MaxPool1(MaxPool1_Out);
    ConvolutionReLU<12, 12, 64, 32> Conv2(convKernel2, convBias2, Conv2_Out);
    MaxPooling<2, 3, 12, 12, 32> MaxPool2(MaxPool2_Out);
    ConvolutionReLU<6, 6, 32, 20> Conv3(convKernel3, convBias3, Conv3_Out);
    MaxPooling<2, 3, 6, 6, 20> MaxPool3(MaxPool3_Out);
    Perceptron<10, 180, 1> Percep4(perceptronKernel4, perceptronBias4, Out);

    std::clog << "Convolution 1" << std::endl;
    Conv1.apply(In);
    std::clog << "MaxPool 1" << std::endl;
    MaxPool1.apply(Conv1.Y);
    std::clog << "Convolution 2" << std::endl;
    Conv2.apply(MaxPool1.Y);
    std::clog << "MaxPool 2" << std::endl;
    MaxPool2.apply(Conv2.Y);
    std::clog << "Convolution 3" << std::endl;
    Conv3.apply(MaxPool2.Y);
    std::clog << "MaxPool 3" << std::endl;
    MaxPool3.apply(Conv3.Y);
    std::clog << "Perceptron 4" << std::endl;
    Percep4.apply(MaxPool3.Y);
}

int main()
{
    // Reads PNG
#ifdef __SIMULATION__
    readPNG("lena.png");
    flattenPNG(ImageIn);
#endif

    layerOut_t completeOut[10];
    applyComplete(ImageIn, completeOut);

#ifdef __SIMULATION__
    unflattenPNG(ImageOut);
    writePNG("out.png");
#else
    for (unsigned int i = 0; i < 10; ++i)
        {
        std::cout << "(" << i << ")\t" << completeOut[i] << std::endl;
        }
#endif

    return 0;
}
