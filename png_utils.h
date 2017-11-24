#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#include <png.h>
#include <fixedpointvariables.h>

#define WIDTH 256
#define HEIGHT 256

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;

png_byte color_type;
png_byte bit_depth;

void readPNG (const char *filename);
void writePNG (const char* filename);

void flattenPNG (layerOut_t *);

template<typename T>
void unflattenPNG (T *I)
{
    for (unsigned int y = 0; y < HEIGHT; ++y)
        {
        for (unsigned int x = 0; x < WIDTH; ++x)
            {
            for (unsigned int c = 0; c < 3; ++c)
                {
                row_pointers[y][x * 3 + c] = (I[y * WIDTH * 3 + x * 3 + c]).slc<INPUT_BITS_PER_PIXEL>
                                             (LAYER_OUTPUT_DYN + LAYER_OUTPUT_PREC - 1 - INPUT_BITS_PER_PIXEL);
                }
            }
        }
}

#endif // PNG_UTILS_H
