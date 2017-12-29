#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<lineBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memInterface<10> > &Out)
{
    static ac_channel<conv1_line_Out_t> Conv1_Out;
    static ac_channel<lineBlockInterface<12 * 64> > MaxPool1_Out;
    static ac_channel<conv2_line_Out_t> Conv2_Out;
    static ac_channel<lineBlockInterface<6 * 32> > MaxPool2_Out;
    static ac_channel<conv3_line_Out_t> Conv3_Out;
    static ac_channel<lineBlockInterface<3 * 20> > MaxPool3_Out;

    conv1_apply(In, Conv1_Out);
    maxPooling_apply<2, 3, 24, 24, 64>(Conv1_Out, MaxPool1_Out);
    conv2_apply(MaxPool1_Out, Conv2_Out);
    maxPooling_apply<2, 3, 12, 12, 32>(Conv2_Out, MaxPool2_Out);
    conv3_apply(MaxPool2_Out, Conv3_Out);
    maxPooling_apply<2, 3, 6, 6, 20>(Conv3_Out, MaxPool3_Out);
    //percep4_apply(MaxPool3_Out, Out);
}

