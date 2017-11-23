#include <iostream>
#include <ac_fixed.h>

#include <convolution.h>
#include <png_utils.h>

#define WIDTH 256
#define HEIGHT 256

uint8 Image[HEIGHT * WIDTH * 3] = {0};
const int KernelImp[3 * 3 * 3 * 3] = {0, 0, 0, 0, 4, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 4, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 4, 0, 0, 0, 0
                                     };

int main()
{
    // Reads PNG
    readPNG("lena.png");
    flattenPNG(Image);

    Convolution C(KernelImp, 3, 3);
    uint8* Y = C.apply(Image, WIDTH, HEIGHT);

    unflattenPNG(Y);
    writePNG("lena2.png");

    return 0;
}
