#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<memInterface<INPUT_SIZE> > &In,
                   ac_channel<memInterface<10> > &Out)
{
    if (!In.available(1))
        {
        return;
        }

    ac_channel<ConvolutionReLU<24, 24, 3, 64>::memOutStruct> Conv1_Out;
    ac_channel<MaxPooling<2, 3, 24, 24, 64>::memOutStruct> MaxPool1_Out;
    ac_channel<ConvolutionReLU<12, 12, 64, 32>::memOutStruct> Conv2_Out;
    ac_channel<MaxPooling<2, 3, 12, 12, 32>::memOutStruct> MaxPool2_Out;
    ac_channel<ConvolutionReLU<6, 6, 32, 20>::memOutStruct> Conv3_Out;
    ac_channel<MaxPooling<2, 3, 6, 6, 20>::memOutStruct> MaxPool3_Out;

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

