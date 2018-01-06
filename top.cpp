#include "convolutionrelu.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memBlockInterface<OUTPUT_SIZE> > &Out,
                   bool selectKernel)
{
    conv_apply(In, Out, selectKernel);
}

