/**
 * @file stenography.h
 * @author Jacob Sharp
 * @copyright MIT License
 *
 * @brief bmp structure and functions for image stenography and manipulation.
 */

#include <stdio.h>

/*****************/
/*** BMP File ****/
/*****************/
/**
 * @brief Stores a bmp photo in the bmp_file structure.
 * @param filename The name of the bmp file.
 * @return Returns a bmp_file structure created from the photo.
 * @note bmp.photo set to NULL when filename is invalid.
 */
bmp_file create_bmp(char *filename);

/**
 * @brief Displays the BMP and DIB headers of a BMP file.
 * @details Takes a bmp photo and prints out the contents of the photo's header.
 * @param photo A bmp photo containing header information.
 */
void display_header(bmp_file bmp);

/**
 * Reveals an image hidden in the least significant bits of a given image.
 */
void reveal(bmp_file bmp);

/********************/
/* Bit Manipulation */
/********************/
/**
 * @brief Swaps the most and least significant bits of a color.
 * @details Swaps the most significant bits with the
 * @param color A single color.
 * @return Returns a color with swapped bits.
 */
char swap_bits(char color);

/**
 * Use masks to combine the MSBs of two different photos.
 */
char combine_bits(char color1, char color2);

/*****************/
/***** Files *****/
/*****************/
void check_read(size_t read, int goal);
void check_seek(int failure, int origin, long offset);

typedef struct
{
    char id[2];
    int file_size;
    short reserved1, reserved2;
    int offset;
} bitmap_file_header;

typedef struct
{
    int header_size, width, height;
    short planes, bpp;
    int scheme, img_size, hres, vres, num_colors, num_imp_colors;
} dib_header;

typedef struct
{
    bitmap_file_header bitmap;
    dib_header dib;
} bmp_header;

typedef struct
{
    bmp_header header;
    FILE *photo;
} bmp_file;