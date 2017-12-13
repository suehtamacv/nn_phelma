#include "convolutionrelu.h"
#include "maxpooling.h"
#include "perceptron.h"
#include "fixedpointvariables.h"
#include "nnarrays.h"

#pragma hls_design top
void applyComplete(ac_channel<memInterface<INPUT_SIZE> > &In,
                   ac_channel<memInterface<10> > &Out)
{
    static ac_channel<ConvolutionReLU<24, 24, 3, 64>::memOutStruct> Conv1_Out;
    static ac_channel<MaxPooling<2, 3, 24, 24, 64>::memOutStruct> MaxPool1_Out;
    static ac_channel<ConvolutionReLU<12, 12, 64, 32>::memOutStruct> Conv2_Out;
    static ac_channel<MaxPooling<2, 3, 12, 12, 32>::memOutStruct> MaxPool2_Out;
    static ac_channel<ConvolutionReLU<6, 6, 32, 20>::memOutStruct> Conv3_Out;
    static ac_channel<MaxPooling<2, 3, 6, 6, 20>::memOutStruct> MaxPool3_Out;

    ConvolutionReLU<24, 24, 3, 64> Conv1("Conv1");
    MaxPooling<2, 3, 24, 24, 64> MaxPool1("MaxPool1");
    ConvolutionReLU<12, 12, 64, 32> Conv2("Conv2");
    MaxPooling<2, 3, 12, 12, 32> MaxPool2("MaxPool2");
    ConvolutionReLU<6, 6, 32, 20> Conv3("Conv3");
    MaxPooling<2, 3, 6, 6, 20> MaxPool3("MaxPool3");
    Perceptron<180, 10> Percep4("Percep4");



    Conv1.apply(In, Conv1_Out, convKernel1, convBias1);
    MaxPool1.apply(Conv1_Out, MaxPool1_Out);
    Conv2.apply(MaxPool1_Out, Conv2_Out, convKernel2, convBias2);
    MaxPool2.apply(Conv2_Out, MaxPool2_Out);
    Conv3.apply(MaxPool2_Out, Conv3_Out, convKernel3, convBias3);
    MaxPool3.apply(Conv3_Out, MaxPool3_Out);
    Percep4.apply(MaxPool3_Out, Out, perceptronKernel4, perceptronBias4);
}

