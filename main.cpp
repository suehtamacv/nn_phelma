#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <mc_scverify.h>

#include "meminterface.h"
#include "nnarrays.h"
#include "png_utils.h"

void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memBlockInterface<OUTPUT_SIZE> > &Out,
                   bool selectKernel);

CCS_MAIN(int argc, char* argv)
{
    ac_channel<memBlockInterface<INPUT_SIZE> > networkInChannel;
    ac_channel<memBlockInterface<OUTPUT_SIZE> > networkOutChannel;
    memBlockInterface<OUTPUT_SIZE> networkOut;

    unsigned int limit = 1;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        readPNG("/calcul/dearamat/lena_160120.png");
        flattenPNG(networkInChannel);

        CCS_DESIGN(applyComplete)(networkInChannel, networkOutChannel, 0);

        networkOut = networkOutChannel.read();
        unflattenPNG(networkOut);
        writePNG("lenaResult.png");
        }

    CCS_RETURN(0);
}
