#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#include <mc_scverify.h>

int readAndNormalize(FILE* image, unsigned int i);
void applyComplete(layerOut_t In[], layerOut_t Out[]);

#pragma hls_design top
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

CCS_MAIN(int argc, char* argv)
{
    FILE* image = fopen("test_batch.bin", "rb");
    layerOut_t networkOut[10];
    double goldenOut[10];

#ifdef __FLOATVERSION__
    std::ofstream Ref("GoldenReference");
#else
    std::ifstream Ref("GoldenReference");
#endif
    std::ofstream Out("OutputClasses");

    double CorrectFound = 0;
    double CorrectFoundGolden = 0;
    unsigned int limit = 1000;
    int goldenLabel;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        int realLabel = readAndNormalize(image, i);

#ifndef __FLOATVERSION__
        // Reads golden version data
        Ref >> goldenLabel;
#endif

        CCS_DESIGN(applyComplete)(ImageIn, networkOut);

        int foundLabel = 0;
        layerOut_t maxFoundLabel = networkOut[0];

        for (unsigned int j = 0; j < 10; ++j)
            {
            if (networkOut[j] > maxFoundLabel)
                {
                maxFoundLabel = networkOut[j];
                foundLabel = j;
                }
            }
#ifdef __FLOATVERSION__
        Ref << foundLabel << std::endl;
#endif
        Out << foundLabel << std::endl;

        if (realLabel == foundLabel)
            {
            std::cout << "Correctly classified " << realLabel << "!" << std::endl;
            CorrectFound++;
            }
        else
            {
            std::cout << "Incorrectly classified " << realLabel << " as " << foundLabel << "." << std::endl;
            }

        if (realLabel == goldenLabel)
            {
            CorrectFoundGolden++;
            }

        }

    std::cout << std::endl;
    std::cout << "Correct rate : " <<
              100 * CorrectFound / (double) limit << "%" << std::endl;
    std::cout << "Correct rate found by golden version : " <<
              100 * CorrectFoundGolden / (double) limit << "%" << std::endl;

    Ref.close();
    Out.close();

    CCS_RETURN(0);
}

int readAndNormalize(FILE* image, unsigned int i)
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
