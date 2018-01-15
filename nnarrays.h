#ifndef NNARRAYS_H
#define NNARRAYS_H

#include "fixedpointvariables.h"

#ifndef WIDTH
#define WIDTH  162
#endif

#ifndef HEIGHT
#define HEIGHT 122
#endif

#define INPUT_SIZE    HEIGHT * WIDTH * 3
#define OUTPUT_SIZE   (HEIGHT - 2) * (WIDTH - 2) * 4

#endif // NNARRAYS_H
