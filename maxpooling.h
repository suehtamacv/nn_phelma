#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include "fixedpointvariables.h"
#include "meminterface.h"

#define stride    2
#define poolSize  3
#define newSizeX  (sizeX / stride)
#define newSizeY  (sizeY / stride)

void maxPooling1_apply(ac_channel<maxPool1_line_In_t> &I, ac_channel<maxPool1_line_Out_t> &Y);
void maxPooling2_apply(ac_channel<maxPool2_line_In_t> &I, ac_channel<maxPool2_line_Out_t> &Y);
void maxPooling3_apply(ac_channel<maxPool3_line_In_t> &I, ac_channel<maxPool3_line_Out_t> &Y);

#endif // MAXPOOLING_H
