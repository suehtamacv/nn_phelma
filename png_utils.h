#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#include <png.h>
#include <fixedpointvariables.h>

#define WIDTH 256
#define HEIGHT 256

static png_structp png_ptr;
static png_infop info_ptr;
static int number_of_passes;
static png_bytep *row_pointers;

static png_byte color_type;
static png_byte bit_depth;

void readPNG (const char *filename);
void writePNG (const char* filename);

void flattenPNG (layerOut_t *);
void unflattenPNG (layerOut_t *I);

#endif // PNG_UTILS_H
