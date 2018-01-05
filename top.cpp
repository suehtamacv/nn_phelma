#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<memBlockInterface<INPUT_SIZE> > &In,
                   ac_channel<memInterface<10> > &Out)
{
    static ac_channel<conv1_line_Out_t> Conv1_Out;
    static ac_channel<maxPool1_line_Out_t> MaxPool1_Out;
    static ac_channel<conv2_line_Out_t> Conv2_Out;
    static ac_channel<maxPool2_line_Out_t> MaxPool2_Out;
    static ac_channel<conv3_line_Out_t> Conv3_Out;
    static ac_channel<maxPool3_line_Out_t> MaxPool3_Out;

    conv1_apply(In, Conv1_Out);
    maxPooling1_apply(Conv1_Out, MaxPool1_Out);
    conv2_apply(MaxPool1_Out, Conv2_Out);
    maxPooling2_apply(Conv2_Out, MaxPool2_Out);
    conv3_apply(MaxPool2_Out, Conv3_Out);
    maxPooling3_apply(Conv3_Out, MaxPool3_Out);
    percep4_apply(MaxPool3_Out, Out);
}

