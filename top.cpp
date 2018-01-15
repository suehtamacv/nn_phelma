#include "convolutionrelu.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memHWCBlockInterface<OUTPUT_SIZE, 3> > &Out,
                   uint2 selectKernel)
{
    conv_apply(In, Out, selectKernel);
}

