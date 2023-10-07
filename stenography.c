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

bmp_file open_bmp(const char *filename)
{
    bmp_file bmp;

    // Open file
    bmp.photo = fopen(filename, "r+");

    if (bmp.photo == NULL)
    {
        fprintf(stderr, "%s not successfully opened.\n", filename);
        return bmp;
    }

    // check proper file type
    checked_read(bmp.header.bitmap.id, sizeof(bmp.header.bitmap.id), 1, bmp.photo);
    if (strncmp(bmp.header.bitmap.id, "BM", 2))
    {
        fprintf(stderr, "%s is the incorrect file type.\n", filename);

        // close the file
        close_bmp(bmp);
        bmp.photo = NULL;
        return bmp;
    }

    // Set bitmap file header values
    checked_read(&bmp.header.bitmap.file_size, sizeof(bmp.header.bitmap.file_size), 1, bmp.photo);
    checked_read(&bmp.header.bitmap.reserved1, sizeof(bmp.header.bitmap.reserved1), 1, bmp.photo);
    checked_read(&bmp.header.bitmap.reserved2, sizeof(bmp.header.bitmap.reserved2), 1, bmp.photo);
    checked_read(&bmp.header.bitmap.offset, sizeof(bmp.header.bitmap.offset), 1, bmp.photo);

    // Set DIB header values
    checked_read(&bmp.header.dib.header_size, sizeof(bmp.header.dib.header_size), 1, bmp.photo);
    checked_read(&bmp.header.dib.width, sizeof(bmp.header.dib.width), 1, bmp.photo);
    checked_read(&bmp.header.dib.height, sizeof(bmp.header.dib.height), 1, bmp.photo);
    checked_read(&bmp.header.dib.planes, sizeof(bmp.header.dib.planes), 1, bmp.photo);
    checked_read(&bmp.header.dib.bpp, sizeof(bmp.header.dib.bpp), 1, bmp.photo);
    checked_read(&bmp.header.dib.scheme, sizeof(bmp.header.dib.scheme), 1, bmp.photo);
    checked_read(&bmp.header.dib.img_size, sizeof(bmp.header.dib.img_size), 1, bmp.photo);
    checked_read(&bmp.header.dib.hres, sizeof(bmp.header.dib.hres), 1, bmp.photo);
    checked_read(&bmp.header.dib.vres, sizeof(bmp.header.dib.vres), 1, bmp.photo);
    checked_read(&bmp.header.dib.num_colors, sizeof(bmp.header.dib.num_colors), 1, bmp.photo);
    checked_read(&bmp.header.dib.num_imp_colors, sizeof(bmp.header.dib.num_imp_colors), 1, bmp.photo);

    return bmp;
}

void close_bmp(bmp_file bmp)
{
    fclose(bmp.photo);
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
        fprintf(stderr, "Program does not handle alternate color densities. Image must be 24 bpp.\n");
        fprintf(stdout, "Photo not revealed.\n");
        return;
    }

    // Create a color structure
    rgb color;

    // Update the photo's colors
    checked_seek(bmp.photo, bmp.header.bitmap.offset, SEEK_SET); // seek to colors
    int padding = (sizeof(rgb) * bmp.header.dib.width) % 4;      // Padding, align row to multiple of 4
    for (int h = 0; h < bmp.header.dib.height; h++)
    {
        for (int w = 0; w < bmp.header.dib.width; w++)
        {
            // Read the next color
            checked_read(&color, sizeof(rgb), 3, bmp.photo);

            // Swap bits of the color
            color.r = swap_bits(color.r);
            color.g = swap_bits(color.g);
            color.b = swap_bits(color.b);

            // Write to the photo
            checked_seek(bmp.photo, -sizeof(rgb), SEEK_CUR);
            fwrite(&color, sizeof(rgb), 1, bmp.photo);
        }

        // End of row padding
        checked_seek(bmp.photo, padding, SEEK_CUR);
    }
}

void hide(bmp_file host, bmp_file hidden)
{
    // Determine RGB Format
    if (host.header.dib.bpp != 24 || hidden.header.dib.bpp != 24)
    {
        fprintf(stderr, "Program does not handle alternate color densities. Image must be 24 bpp.\n");
        fprintf(stdout, "Photo not revealed.\n");
        return;
    }

    // Make sure same size
    if (host.header.dib.height != hidden.header.dib.height ||
        host.header.dib.width != hidden.header.dib.width)
    {
        fprintf(stderr, "The two photos are not the same size. Images must be the same height and width.\n");
        fprintf(stdout, "Photo not stored.\n");
        return;
    }

    // Create a color structure
    rgb host_color;
    rgb hidden_color;

    // Update the photo's colors
    checked_seek(host.photo, host.header.bitmap.offset, SEEK_SET); // seek to colors
    checked_seek(hidden.photo, hidden.header.bitmap.offset, SEEK_SET);

    int padding = (sizeof(host_color) * host.header.dib.width) % 4; // Padding, align row to multiple of 4
    for (int h = 0; h < host.header.dib.height; h++)                // padding, height, and width are the same for each photo
    {
        for (int w = 0; w < host.header.dib.width; w++)
        {
            // Read the next color
            checked_read(&host_color, sizeof(host_color.r), 3, host.photo);
            checked_read(&hidden_color, sizeof(hidden_color.r), 3, hidden.photo);

            // Swap bits of  color
            host_color.r = combine_bits(host_color.r, hidden_color.r);
            host_color.g = combine_bits(host_color.g, hidden_color.g);
            host_color.b = combine_bits(host_color.b, hidden_color.b);

            // Write to the photo
            checked_seek(host.photo, -sizeof(host_color), SEEK_CUR);
            fwrite(&host_color, sizeof(host_color), 1, host.photo);
        }

        // End of row padding
        checked_seek(host.photo, padding, SEEK_CUR);
    }
}

void invert(bmp_file bmp)
{
    // Validate bmp format
    if (!validate_bpp(bmp.header.dib.bpp))
    {
        fprintf(stdout, "Photo was not inverted.\n");
        return;
    }

    // Create a color structure
    rgb color;

    // Update the photo's colors
    checked_seek(bmp.photo, bmp.header.bitmap.offset, SEEK_SET); // jump to pixels
    int padding = (sizeof(rgb) * bmp.header.dib.width) % 4;      // padding aligns row to multiple of 4
    for (int h = 0; h < bmp.header.dib.height; h++)
    {
        for (int w = 0; w < bmp.header.dib.width; w++)
        {
            // Read the next color
            checked_read(&color, sizeof(color.r), 3, bmp.photo);

            // Invert the color
            color.r = invert_bits(color.r);
            color.g = invert_bits(color.g);
            color.b = invert_bits(color.b);

            // Write to the photo
            checked_seek(bmp.photo, -sizeof(rgb), SEEK_CUR);
            fwrite(&color, sizeof(rgb), 1, bmp.photo);
        }

        // End of row padding
        checked_seek(bmp.photo, padding, SEEK_CUR);
    }
}

void hflip_image(bmp_file bmp)
{
    // Validate bmp format
    if (!validate_bpp(bmp.header.dib.bpp))
    {
        fprintf(stdout, "Photo was not flipped.\n");
        return;
    }

    checked_seek(bmp.photo, bmp.header.bitmap.offset, SEEK_SET); // jump to pixels
    int row_size = bmp.header.dib.width * sizeof(rgb);
    rgb row[bmp.header.dib.width];
    int padding = row_size % 4; // padding aligns row to multiple of 4

    for (int h = 0; h < bmp.header.dib.height; h++)
    {
        // Store the entire row
        checked_read(row, sizeof(rgb), bmp.header.dib.width, bmp.photo);

        // Swap the colors
        for (int w = 0; w < bmp.header.dib.width / 2; w++)
        {
            printf("Before Swap: %d %d %d - %d %d %d\n", row[w].r, row[w].g, row[w].b, row[bmp.header.dib.width - w - 1].r, row[bmp.header.dib.width - w - 1].g, row[bmp.header.dib.width - w - 1].b);
            swap(&row[w], &row[bmp.header.dib.width - w - 1]);
            printf("After Swap: %d %d %d - %d %d %d\n", row[w].r, row[w].g, row[w].b, row[bmp.header.dib.width - w - 1].r, row[bmp.header.dib.width - w - 1].g, row[bmp.header.dib.width - w - 1].b);
        }

        // Write to the photo
        checked_seek(bmp.photo, -row_size, SEEK_CUR);
        fwrite(&row, row_size, 1, bmp.photo);

        // Add the end of row padding
        checked_seek(bmp.photo, padding, SEEK_CUR);
    }
}

void mirror(bmp_file bmp)
{
    // Validate bmp format
    if (!validate_bpp(bmp.header.dib.bpp))
    {
        fprintf(stdout, "Photo was not flipped.\n");
        return;
    }

    checked_seek(bmp.photo, bmp.header.bitmap.offset, SEEK_SET); // jump to pixels
    int row_size = bmp.header.dib.width * sizeof(rgb);
    rgb row[bmp.header.dib.width];
    int padding = row_size % 4; // padding aligns row to multiple of 4

    for (int h = 0; h < bmp.header.dib.height; h++)
    {
        // Store the entire row
        checked_read(row, sizeof(rgb), bmp.header.dib.width, bmp.photo);

        // Copy the colors
        for (int w = 0; w < bmp.header.dib.width / 2; w++)
        {
            copy(&row[w], &row[bmp.header.dib.width - w - 1]);
        }

        // Write to the photo
        checked_seek(bmp.photo, -row_size, SEEK_CUR);
        fwrite(&row, row_size, 1, bmp.photo);

        // Add the end of row padding
        checked_seek(bmp.photo, padding, SEEK_CUR);
    }
}

/****************************************/
/*********** Bit Manipulation ***********/
/****************************************/
void swap(rgb *color1, rgb *color2)
{
    rgb temp = *color1;
    *color1 = *color2;
    *color2 = temp;
}

void copy(rgb *color1, rgb *color2)
{
    *color2 = *color1;
}

char swap_bits(char color)
{
    unsigned char msb, lsb, temp; // avoid signed ext

    // split color
    msb = 0xF0 & color;
    lsb = 0x0F & color;

    // switch order
    temp = msb;
    msb = lsb << 4;
    lsb = temp >> 4; // 1111MSB
    return lsb | msb;
}

char combine_bits(char color1, char color2)
{
    unsigned char msb, lsb; // avoid signed extention

    // capture MSB of each color
    msb = 0xF0 & color1;
    lsb = 0xF0 & color2;

    // combine colors
    lsb = lsb >> 4;
    return msb | lsb;
}

char invert_bits(char color)
{
    return ~color;
}

/****************************************/
/**************** Files *****************/
/****************************************/
void checked_read(void *restrict __ptr, size_t __size, size_t __nitems, FILE *restrict __stream)
{
    int read = fread(__ptr, __size, __nitems, __stream);
    if ((int)read != __nitems)
    {
        if ((int)read == 0)
        {
            fprintf(stderr, "No elements were read.\n");
        }
        else
        {
            fprintf(stderr, "Not all elements were read.\n");
        }
    }
}

void checked_write(void *restrict __ptr, size_t __size, size_t __nitems, FILE *restrict __stream)
{
    int written = fwrite(__ptr, __size, __nitems, __stream);
    if ((int)written != __nitems)
    {
        if ((int)written == 0)
        {
            fprintf(stderr, "No elements were written.\n");
        }
        else
        {
            fprintf(stderr, "Not all elements were written.\n");
        }
    }
}

void checked_seek(FILE *file, long int offset, int whence)
{
    int temp_whence = whence;
    int result = fseek(file, offset, whence);
    if (result)
    {
        if (offset >= 0)
        {
            fprintf(stderr, "Failed to seek from %i forward %ld", temp_whence, offset);
        }
        else
        {
            fprintf(stderr, "Failed to seek from %i backwards %ld", temp_whence, offset);
        }
    }
}

/****************************************/
/************** Validation **************/
/****************************************/
int validate_bpp(int bpp)
{
    if (bpp != 24)
    {
        fprintf(stderr, "Program does not handle alternate color densities. Image must be 24 bpp.\n");
        fprintf(stdout, "Photo has an invalid color density.\n");
        return 0;
    }
    return 1;
}