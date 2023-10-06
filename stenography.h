/**
 * @file stenography.h
 * @author Jacob Sharp
 * @copyright MIT License
 *
 * @brief bmp structure and functions for image stenography and manipulation.
 */

#include <stdio.h>

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

typedef struct
{
    char r, g, b;
} rgb;

/*****************/
/*** BMP File ****/
/*****************/
/**
 * @brief Stores a bmp photo in the bmp_file structure.
 * @param filename The name of the bmp file.
 * @return Returns a bmp_file structure created from the photo.
 * @note bmp.photo set to NULL when filename is invalid.
 */
bmp_file open_bmp(const char *filename);

/**
 * @brief Closes the bmp file.
 * @param bmp bmp file to close
 */
void close_bmp(bmp_file bmp);

/**
 * @brief Displays the BMP and DIB headers of a BMP file.
 * @details Takes a bmp photo and prints out the contents of the photo's header.
 * @param bmp a bmp photo
 */
void display_header(bmp_file bmp);

/*****************/
/*** Alter BMP ***/
/*****************/
/**
 * @brief Reveals an photo hidden in the LSbs of an photo.
 * @details Alters the original photo by swapping its MSbs and LSbs and revealing the hidden photo.
 * @param bmp bmp photo containing a hidden photo.
 */
void reveal(bmp_file bmp);

/**
 * @brief Hides one photo inside of another photo.
 * @details Stores the MSbs of the hidden photo as the LSbs of the target photo.
 * @param target Target bmp photo which will hide the other photo.
 * @param hidden bmp photo to hide inside of the target photo
 */
void hide(bmp_file target, bmp_file hidden);

void invert_image(const char *in_filename);

void grayscale_image(const char *in_filename);

void hflip_image(const char *in_filename);

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
 * Use masks to combine the MSbs of two different photos.
 */
char combine_bits(char color1, char color2);

/*****************/
/***** Files *****/
/*****************/

/**
 * @brief Performs fseek and writes to stderr upon failure.
 * @param file Pointer to the file stream.
 * @param offset Number of bytes from position of `whence`.
 * @param whence Position used as reference for offset: SEEK_SET, SEEK_CUR, or SEEK_END
 */
void checked_seek(FILE *file, long int offset, int whence);
/**
 * @brief Performs fread and writes to stderr upon failure.
 * @param __ptr Pointer to where the read data should be copied.
 * @param __size Size of the data.
 * @param __nitems Number of items to be read.
 * @param __stream File stream from which data is being read.
 */
void checked_read(void *__restrict__ __ptr, size_t __size, size_t __nitems, FILE *__restrict__ __stream);
/**
 * @brief Performs fwrite and writes to stderr upon failure.
 * @param __ptr Pointer to where the data should be written.
 * @param __size Size of the data.
 * @param __nitems Number of items to be written.
 * @param __stream File stream from which data is being written.
 */
void checked_write(void *restrict __ptr, size_t __size, size_t __nitems, FILE *restrict __stream);