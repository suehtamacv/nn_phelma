#include <png_utils.h>
#include <stdlib.h>
#include <iostream>

void readPNG(const char* filename)
{
    char header[8];
    // open file and test for it being a png
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        {
        abort();
        }
    fread(header, 1, 8, fp);

    // initialize stuff
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        {
        abort();
        }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        {
        abort();
        }

    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    // read file
    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    for (unsigned int y = 0; y < (HEIGHT - 2); y++)
        {
        row_pointers[y] = new png_byte[(WIDTH - 2) * 3];
        }

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
}

void writePNG(const char* file_name)
{
    // create file
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
        {
        abort();
        }

    // initialize stuff
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        {
        abort();
        }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        {
        abort();
        }

    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    png_init_io(png_ptr, fp);

    // write header
    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    png_set_IHDR(png_ptr, info_ptr, (WIDTH - 2), (HEIGHT - 2),
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    // write bytes
    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    png_write_image(png_ptr, row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        {
        abort();
        }

    png_write_end(png_ptr, NULL);

    // cleanup heap allocation
    for (unsigned int y = 0; y < (HEIGHT - 2); y++)
        {
        delete[] row_pointers[y];
        }

    fclose(fp);
}


void flattenPNG(ac_channel<memBlockInterface<INPUT_SIZE> > &channelI)
{
    memBlockInterface<INPUT_SIZE> Img;

    for (unsigned int i = 0; i < (HEIGHT / BLOCK_HEIGHT) * (WIDTH / BLOCK_WIDTH) * 3; ++i)
        {
        Img.Y[i][0] = 0;
        Img.Y[i][1] = 0;
        Img.Y[i][2] = 0;
        Img.Y[i][3] = 0;
        }

    for (unsigned int yI = 0; yI < (HEIGHT - BLOCK_HEIGHT); ++yI)
        {
        for (unsigned int xI = 0; xI < (WIDTH - BLOCK_WIDTH); ++xI)
            {
            for (unsigned int cI = 0; cI < 3; ++cI)
                {
                unsigned int indexBck = cI * ((HEIGHT - BLOCK_HEIGHT) / BLOCK_HEIGHT) * ((WIDTH - BLOCK_WIDTH) / BLOCK_WIDTH)
                                        + (yI / BLOCK_WIDTH) * ((WIDTH - BLOCK_WIDTH) / BLOCK_WIDTH)
                                        + (xI / BLOCK_HEIGHT);
                unsigned int index = ((yI + 1) % 2) * 2 + (xI + 1) % 2;
                Img.Y[indexBck][index] = row_pointers[yI][xI * 3 + cI];
                }
            }
        }

    channelI.write(Img);
}

void unflattenPNG(memBlockInterface<OUTPUT_SIZE> &Img)
{
    for (unsigned int yI = 0; yI < (HEIGHT - BLOCK_HEIGHT); ++yI)
        {
        row_pointers[yI] = new png_byte[(WIDTH - BLOCK_WIDTH) * 3];

        for (unsigned int xI = 0; xI < (WIDTH - BLOCK_WIDTH); ++xI)
            {
            for (unsigned int cI = 0; cI < 3; ++cI)
                {
                unsigned int indexBck = cI * ((HEIGHT - BLOCK_HEIGHT) / BLOCK_HEIGHT) * ((WIDTH - BLOCK_WIDTH) / BLOCK_WIDTH)
                                        + (yI / BLOCK_WIDTH) * ((WIDTH - BLOCK_WIDTH) / BLOCK_WIDTH)
                                        + (xI / BLOCK_HEIGHT);
                unsigned int index = ((yI + 1) % 2) * 2 + (xI + 1) % 2;

                row_pointers[yI][xI * 3 + cI] = (Img.Y[indexBck][index]).slc<8>(PIXEL_DYN + PIXEL_PREC - 8);
                }
            }
        }

}
