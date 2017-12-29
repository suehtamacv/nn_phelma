#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "meminterface.h"
#include "nnarrays.h"
#include <mc_scverify.h>

int readAndNormalize(FILE* image, ac_channel<lineBlockInterface<INPUT_SIZE> > &Y, unsigned int i);
void applyComplete(ac_channel<lineBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memInterface<10> > &Out);

CCS_MAIN(int argc, char* argv)
{
    FILE* image = fopen("test_batch.bin", "rb");
    ac_channel<lineBlockInterface<INPUT_SIZE> > networkInChannel;
    ac_channel<memInterface<10> > networkOutChannel;
    memInterface<10> networkOut;

#ifdef __FLOATVERSION__
    std::ofstream Ref("GoldenReference");
#else
    std::ifstream Ref("GoldenReference");
#endif
    std::ofstream Out("OutputClasses");

    double CorrectFound = 0;
    double CorrectFoundGolden = 0;
    unsigned int limit = 5000;
    int goldenLabel;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        int realLabel = readAndNormalize(image, networkInChannel, i);

#ifndef __FLOATVERSION__
        // Reads golden version data
        Ref >> goldenLabel;
#endif

        CCS_DESIGN(applyComplete)(networkInChannel, networkOutChannel);

        networkOut = networkOutChannel.read();

        int foundLabel = 0;
        pixel_t maxFoundLabel = networkOut.Y[0];

        for (unsigned int j = 0; j < 10; ++j)
            {
            if (networkOut.Y[j] > maxFoundLabel)
                {
                maxFoundLabel = networkOut.Y[j];
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

int readAndNormalize(FILE* image, ac_channel<lineBlockInterface<INPUT_SIZE> > &Y, unsigned int i)
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

    for (unsigned int yI = 0; yI < 24; yI += 2)
        {
        lineBlockInterface<INPUT_SIZE> line;

        for (unsigned int cI = 0; cI < 3; ++cI)
            {
            for (unsigned int xI = 0; xI < 24; xI += 2)
                {

                layerOutBlock_t out;
                for (unsigned int off_yI = 0; off_yI < 2; ++off_yI)
                    {
                    for (unsigned int off_xI = 0; off_xI < 2; ++off_xI)
                        {
                        out.P[off_yI * 2 + off_xI] = (ImageData[cI * 32 * 32 + (yI + off_yI + 4) * 32 +
                                                                (xI + off_xI + 4)] - Average) / std::max(StdDev, minStdDev);
                        }
                    }
                line.Y[cI * 12 + xI / 2] = out;
                }
            }

        Y.write(line);
        }

    return ImageLabel;
}
