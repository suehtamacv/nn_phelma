#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#include <png.h>
#include "fixedpointvariables.h"
#include "meminterface.h"

static png_structp png_ptr;
static png_infop info_ptr;
static int number_of_passes;
static png_bytep row_pointers[HEIGHT];

static png_byte color_type;
static png_byte bit_depth;

void readPNG(const char *filename);
void writePNG(const char* filename);

void flattenPNG(ac_channel<memBlockInterface<INPUT_SIZE> > &channelI);
void unflattenPNG(memHWCBlockInterface<OUTPUT_SIZE, 4> &Img);

#endif // PNG_UTILS_H
