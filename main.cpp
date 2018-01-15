#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <mc_scverify.h>

#include "meminterface.h"
#include "nnarrays.h"
#include "png_utils.h"

void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memHWCBlockInterface<OUTPUT_SIZE, 3> > &Out,
                   uint2 selectKernel);

CCS_MAIN(int argc, char* argv)
{
    ac_channel<memBlockInterface<INPUT_SIZE> > networkInChannel;
    ac_channel<memHWCBlockInterface<OUTPUT_SIZE, 3> > networkOutChannel;
    memHWCBlockInterface<OUTPUT_SIZE, 3> networkOut;

    unsigned int limit = 1;

    for (unsigned int i = 0; i < limit; ++i)
        {
        // Reads RAW
        readPNG("lena_160120.png");
        flattenPNG(networkInChannel);

        CCS_DESIGN(applyComplete)(networkInChannel, networkOutChannel, 3);

        networkOut = networkOutChannel.read();
        unflattenPNG(networkOut);
        writePNG("lenaResult.png");
        }

    CCS_RETURN(0);
}
