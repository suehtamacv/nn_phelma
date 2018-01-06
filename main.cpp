#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "meminterface.h"
#include "nnarrays.h"
#include <mc_scverify.h>

int readAndNormalize(FILE* image, ac_channel<memBlockInterface<INPUT_SIZE> > &Y, unsigned int i);
void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memBlockInterface<OUTPUT_SIZE> > &Out,
                   bool selectKernel);

CCS_MAIN(int argc, char* argv)
{
    FILE* image = fopen("test_batch.bin", "rb");
    ac_channel<memBlockInterface<INPUT_SIZE> > networkInChannel;
    ac_channel<memBlockInterface<OUTPUT_SIZE> > networkOutChannel;
    memBlockInterface<OUTPUT_SIZE> networkOut;

    unsigned int limit = 1;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        readAndNormalize(image, networkInChannel, i);

        CCS_DESIGN(applyComplete)(networkInChannel, networkOutChannel, true);

        networkOut = networkOutChannel.read();
        }

    CCS_RETURN(0);
}

int readAndNormalize(FILE* image, ac_channel<memBlockInterface<INPUT_SIZE> > &Y, unsigned int i)
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

    memBlockInterface<INPUT_SIZE> mem;
    for (unsigned int yI = 0; yI < HEIGHT; yI += BLOCK_HEIGHT)
        {
        for (unsigned int cI = 0; cI < 3; ++cI)
            {
            for (unsigned int xI = 0; xI < WIDTH; xI += BLOCK_WIDTH)
                {
                for (unsigned int off_yI = 0; off_yI < BLOCK_HEIGHT; ++off_yI)
                    {
                    for (unsigned int off_xI = 0; off_xI < BLOCK_WIDTH; ++off_xI)
                        {
                        unsigned int bckIndex = cI * (HEIGHT / BLOCK_HEIGHT) * (WIDTH / BLOCK_WIDTH) +
                                                (yI / BLOCK_HEIGHT) * (WIDTH / BLOCK_WIDTH) +
                                                (xI / BLOCK_WIDTH);
                        if ((yI + off_yI == 0) || (yI + off_yI == HEIGHT - 1))
                            {
                            mem.Y[bckIndex][off_yI * BLOCK_WIDTH + off_xI] = 0;
                            }
                        else if ((xI + off_xI == 0) || (xI + off_xI == WIDTH - 1))
                            {
                            mem.Y[bckIndex][off_yI * BLOCK_WIDTH + off_xI] = 0;
                            }
                        else
                            {
                            mem.Y[bckIndex][off_yI * BLOCK_WIDTH + off_xI] =
                                (ImageData[cI * 32 * 32 + (yI + off_yI + 3) * 32 + (xI + off_xI + 3)] - Average)
                                / std::max(StdDev, minStdDev);
                            }
                        }
                    }
                }
            }
        }

    Y.write(mem);
    return ImageLabel;
}
