#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#define PNG_DEBUG 3
#include <png.h>
#include <ac_fixed.h>

void readPNG (const char *filename);
void flattenPNG (int *);
void unflattenPNG (int *I);
void writePNG (const char* filename);

#endif // PNG_UTILS_H
