#ifdef __SIMULATION__

#include <iostream>

#include "softmax.h"
#include "png_utils.h"

FILE* image;

#endif

#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#include <stdlib.h>

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

int readAndNormalize(unsigned int i)
{
    char ImageData[32 * 32 * 3];
    char ImageLabel;

    double Average = 0;
    double StdDev = 0;
    const double minStdDev = 1.0 / sqrt(24 * 24 * 3);

    fread(&ImageLabel, 1, 1, image);
    fread(ImageData, 1, 32 * 32 * 3, image);

    for (unsigned int cI = 0; cI < 3; ++cI)
        {
        for (unsigned int yI = 0; yI < 24; ++yI)
            {
            for (unsigned int xI = 0; xI < 24; ++xI)
                {
                Average += ImageData[cI * 32 * 32 + (yI + 4) * 32 + (xI + 4)];
                }
            }
        }
    Average /= (24 * 24 * 3);

    for (unsigned int cI = 0; cI < 3; ++cI)
        {
        for (unsigned int yI = 0; yI < 24; ++yI)
            {
            for (unsigned int xI = 0; xI < 24; ++xI)
                {
                StdDev += pow((ImageData[cI * 32 * 32 + (yI + 4) * 32 + (xI + 4)] - Average), 2);
                }
            }
        }
    StdDev = sqrt(StdDev / (24 * 24 * 3));

    for (unsigned int cI = 0; cI < 3; ++cI)
        {
        for (unsigned int yI = 0; yI < 24; ++yI)
            {
            for (unsigned int xI = 0; xI < 24; ++xI)
                {
                ImageIn[cI * 24 * 24 + yI * 24 + xI] =
                    (ImageData[cI * 32 * 32 + (yI + 4) * 32 + (xI + 4)] - Average) / std::max(StdDev, minStdDev);
                }
            }
        }

    return ImageLabel;
}

int main()
{
    image = fopen("data_batch_1.bin", "rb");
    layerOut_t completeOut[10];

    double CorrectFound = 0;
    unsigned int limit = 1000;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        int goldenLabel = readAndNormalize(i);

        applyComplete(ImageIn, completeOut);
        int foundLabel = 0;
        layerOut_t maxFoundLabel = completeOut[0];

        for (unsigned int i = 1; i < 10; ++i)
            {
            if (completeOut[i] > maxFoundLabel)
                {
                maxFoundLabel = completeOut[i];
                foundLabel = i;
                }
            }

        if (goldenLabel == foundLabel)
            {
            std::cout << "Correctly classified " << goldenLabel << "!" << std::endl;
            CorrectFound++;
            }
        else
            {
            std::cout << "Incorrectly classified " << goldenLabel << " as " << foundLabel << "." << std::endl;
            }

        }

    std::cout << "Correct examples" << CorrectFound / (double) limit << std::endl;

    return 0;
}
