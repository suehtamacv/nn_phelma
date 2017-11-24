#include <iostream>

#include <fixedpointvariables.h>

#include <convolution.h>
#include <maxpooling.h>
#include <softmax.h>
#include <png_utils.h>

#define WIDTH 256
#define HEIGHT 256

img_t Image[HEIGHT * WIDTH * 3] = {0};
const kernel_t KernelImp[3 * 3 * 3 * 3] = {0, 0, 0, 0, 1, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 1, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 1, 0, 0, 0, 0
                                          };

const kernel_t Gaussian[3 * 3 * 3 * 3] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16
                                         };


int main()
{
    // Reads PNG
    readPNG("lena.png");
    flattenPNG(Image);

    unflattenPNG(Image);
    writePNG("lena2.png");

    return 0;
}
