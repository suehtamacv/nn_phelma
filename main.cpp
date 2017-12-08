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

convKernel_t convKernel[] = {0, 0, 0, 0, 1, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 1, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 1, 0, 0, 0, 0
                            };
convBias_t   convBias[]   = {0, 0, 0};

#pragma hls_design top
void apply(layerOut_t In[256 * 256 * 3], layerOut_t Out[256 * 256 * 3])
{
    ConvolutionReLU<256, 256, 3, 3> Conv1("Conv1", convKernel, convBias, Out);
    Conv1.apply(In);
}

#ifdef __SIMULATION__

void applyComplete(layerOut_t In[24 * 24 * 3], layerOut_t Out[10])
{
    layerOut_t Conv1_Out[24 * 24 * 64];
    layerOut_t MaxPool1_Out[12 * 12 * 64];
    layerOut_t Conv2_Out[12 * 12 * 32];
    layerOut_t MaxPool2_Out[6 * 6 * 32];
    layerOut_t Conv3_Out[6 * 6 * 20];
    layerOut_t MaxPool3_Out[3 * 3 * 20];

    ConvolutionReLU<24, 24, 3, 64> Conv1("Conv1", convKernel1, convBias1, Conv1_Out);
    MaxPooling<2, 3, 24, 24, 64> MaxPool1("MaxPool1", MaxPool1_Out);
    ConvolutionReLU<12, 12, 64, 32> Conv2("Conv2", convKernel2, convBias2, Conv2_Out);
    MaxPooling<2, 3, 12, 12, 32> MaxPool2("MaxPool2", MaxPool2_Out);
    ConvolutionReLU<6, 6, 32, 20> Conv3("Conv3", convKernel3, convBias3, Conv3_Out);
    MaxPooling<2, 3, 6, 6, 20> MaxPool3("MaxPool3", MaxPool3_Out);
    Perceptron<180, 10> Percep4("Percep4", perceptronKernel4, perceptronBias4, Out);

    Conv1.apply(In);
    MaxPool1.apply(Conv1.Y);
    Conv2.apply(MaxPool1.Y);
    MaxPool2.apply(Conv2.Y);
    Conv3.apply(MaxPool2.Y);
    MaxPool3.apply(Conv3.Y);
    Percep4.apply(MaxPool3.Y);
}

int readAndNormalize(unsigned int i)
{
    unsigned char ImageData[32 * 32 * 3];
    unsigned char ImageLabel;

    double Average = 0;
    double StdDev = 0;
    const double minStdDev = 1.0 / sqrt(24 * 24 * 3);

    fseek(image, 3073 * i, SEEK_SET);
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
#ifdef __HWC__
                ImageIn[yI * 24 * 3 + xI * 3 + cI] =
#else
                ImageIn[cI * 24 * 24 + yI * 24 + xI] =
#endif
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
#ifdef __STAT__
    unsigned int limit = 1;
#else
    unsigned int limit = 1000;
#endif

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

    std::cout << "Correct examples " << CorrectFound / (double) limit << std::endl;

    return 0;
}

#endif
