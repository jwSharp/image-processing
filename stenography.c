/**
 * @file stenography.h
 * @author Jacob Sharp
 * @copyright MIT License
 *
 * @brief bmp structure and functions for image stenography and manipulation.
 */

#include <stdio.h>
#include <string.h>
#include "stenography.h"

/****************************************/
/*************** BMP File ***************/
/****************************************/

bmp_file create_bmp(char *filename)
{
    bmp_file bmp;

    // Open file
    bmp.photo = fopen(filename, "r+");
    if (bmp.photo == NULL)
    {
        fprintf(stderr, "Photo not successfully opened.");
        return bmp;
    }

    // Set bitmap file header values
    check_read(fread(bmp.header.bitmap.id, sizeof(bmp.header.bitmap.id), 1, bmp.photo), 1);

    if (strncmp(bmp.header.bitmap.id, "BM", 2))
    {
        fprintf(stderr, "Incorrect file type.");
        bmp.photo = NULL;
        return bmp;
    }
    check_read(fread(&bmp.header.bitmap.file_size, sizeof(bmp.header.bitmap.file_size), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.bitmap.reserved1, sizeof(bmp.header.bitmap.reserved1), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.bitmap.reserved2, sizeof(bmp.header.bitmap.reserved2), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.bitmap.offset, sizeof(bmp.header.bitmap.offset), 1, bmp.photo), 1);

    // Set DIB header values
    check_read(fread(&bmp.header.dib.header_size, sizeof(bmp.header.dib.header_size), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.width, sizeof(bmp.header.dib.width), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.height, sizeof(bmp.header.dib.height), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.planes, sizeof(bmp.header.dib.planes), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.bpp, sizeof(bmp.header.dib.bpp), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.scheme, sizeof(bmp.header.dib.scheme), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.img_size, sizeof(bmp.header.dib.img_size), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.hres, sizeof(bmp.header.dib.hres), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.vres, sizeof(bmp.header.dib.vres), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.num_colors, sizeof(bmp.header.dib.num_colors), 1, bmp.photo), 1);
    check_read(fread(&bmp.header.dib.num_imp_colors, sizeof(bmp.header.dib.num_imp_colors), 1, bmp.photo), 1);

    return bmp;
}

void display_header(bmp_file bmp)
{
    // Print BMP header details
    fprintf(stdout, "=== BMP Header ===\n");
    fprintf(stdout, "Type: %.2s\n", bmp.header.bitmap.id);
    fprintf(stdout, "Size: %i\n", bmp.header.bitmap.file_size);
    fprintf(stdout, "Reserved 1: %i\n", bmp.header.bitmap.reserved1);
    fprintf(stdout, "Reserved 2: %i\n", bmp.header.bitmap.reserved2);
    fprintf(stdout, "Image offset: %i\n", bmp.header.bitmap.offset);

    // Print DIB header details
    fprintf(stdout, "\n=== DIB Header ===\n");
    fprintf(stdout, "Size: %i\n", bmp.header.dib.header_size);
    fprintf(stdout, "Width: %i\n", bmp.header.dib.width);
    fprintf(stdout, "Height: %i\n", bmp.header.dib.height);
    fprintf(stdout, "# color planes: %i\n", bmp.header.dib.planes);
    fprintf(stdout, "# bits per pixel: %i\n", bmp.header.dib.bpp);
    fprintf(stdout, "Compression scheme: %i\n", bmp.header.dib.scheme);
    fprintf(stdout, "Image size: %i\n", bmp.header.dib.img_size);
    fprintf(stdout, "Horizontal resolution: %i\n", bmp.header.dib.hres);
    fprintf(stdout, "Vertical resolution: %i\n", bmp.header.dib.vres);
    fprintf(stdout, "# colors in palette: %i\n", bmp.header.dib.num_colors);
    fprintf(stdout, "# important colors: %i\n", bmp.header.dib.num_imp_colors);
}

void reveal(bmp_file bmp)
{

    // Determine RGB Format
    if (bmp.header.dib.bpp != 24)
    {
        fprintf(stderr, "Program does not handle alternate color densities. Image must be 24 bpp.");
        fprintf(stdout, "Photo not revealed.");
        return;
    }
    typedef struct
    {
        char r, g, b;
    } rgb;

    //
    // Alter Colors
    check_seek(fseek(bmp.photo, bmp.header.bitmap.offset, SEEK_SET), 0, bmp.header.bitmap.offset); // from beginning of file
    rgb color;
    int padding = (sizeof(color) * bmp.header.dib.width) % 4; // Padding, align row to multiple of 4

    for (int h = 0; h < bmp.header.dib.height; h++)
    {
        for (int w = 0; w < bmp.header.dib.width; w++)
        {
            check_read(fread(&color, sizeof(color.r), 3, bmp.photo), 3);
            rgb before = color;

            // Swap Bits
            color.r = swap_bits(color.r);
            color.g = swap_bits(color.g);
            color.b = swap_bits(color.b);

            // Update photo
            check_seek(fseek(bmp.photo, -sizeof(color), SEEK_CUR), SEEK_CUR, -sizeof(color.r) * 3);
            fwrite(&color, sizeof(color), 1, bmp.photo);

            check_seek(fseek(bmp.photo, -sizeof(color), SEEK_CUR), SEEK_CUR, -sizeof(color.r) * 3);
            check_read(fread(&color, sizeof(color.r), 3, bmp.photo), 3);
            rgb after = color;
            int is_same;
            if (before.r == after.r)
            {
                is_same = 1;
            }
            else
            {
                is_same = 0;
            }
            fprintf(stderr, "%i", is_same);
        }

        check_seek(fseek(bmp.photo, padding, SEEK_CUR), SEEK_CUR, padding); // End of row padding
    }
}

/****************************************/
/*********** Bit Manipulation ***********/
/****************************************/

char swap_bits(char color)
{
    char msb, lsb, temp;

    // split color
    msb = 0xF0 & color;
    lsb = 0x0F & color;

    // switch order
    temp = msb;
    msb = lsb << 4;
    lsb = temp >> 4;
    return lsb | msb;
}

char combine_bits(char color1, char color2)
{
    char msb, lsb;

    // capture MSB of each color
    msb = 0xF0 & color1;
    lsb = 0xF0 & color2;

    // combine colors
    lsb = lsb >> 4;
    return msb | lsb;
}

/****************************************/
/**************** Files *****************/
/****************************************/

/**
 * Checks for proper number of successfully read objects.
 */
void check_read(size_t read, int goal)
{
    if ((int)read != goal)
    {
        if ((int)read > 0)
        {
            fprintf(stderr, "Not all elements were read.");
        }
        else
        {
            fprintf(stderr, "No elements were read.");
        }
    }
}

/**
 *
 */
void check_seek(int failure, int origin, long offset)
{
    if (failure)
    {
        if (offset >= 0)
        {
            fprintf(stderr, "Failed to seek from %i forward %ld", origin, offset);
        }
        else
        {
            fprintf(stderr, "Failed to seek from %i backwards %ld", origin, offset);
        }
    }
}