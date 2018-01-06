#ifndef NNARRAYS_H
#define NNARRAYS_H

#include "fixedpointvariables.h"

#ifndef WIDTH
#define WIDTH  34
#endif

#ifndef HEIGHT
#define HEIGHT 34
#endif

#define INPUT_SIZE    HEIGHT * WIDTH * 3
#define OUTPUT_SIZE   (HEIGHT - 2) * (WIDTH - 2) * 3

#endif // NNARRAYS_H
