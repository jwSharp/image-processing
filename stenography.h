/**
 * @file stenography.h
 * @author Jacob Sharp
 * @copyright MIT License
 *
 * @brief bmp structure and functions for image stenography and manipulation.
 */

#include <stdio.h>

/**
 * The contents of a bmp's bitmap header
 */
typedef struct
{
    char id[2];
    int file_size;
    short reserved1, reserved2;
    int offset;
} bitmap_file_header;
/**
 * The contents of a bmp's dib header
 */
typedef struct
{
    int header_size, width, height;
    short planes, bpp;
    int scheme, img_size, hres, vres, num_colors, num_imp_colors;
} dib_header;
/**
 * The header for a bmp file
 */
typedef struct
{
    bitmap_file_header bitmap;
    dib_header dib;
} bmp_header;
/**
 * BMP file and its header
 */
typedef struct
{
    bmp_header header;
    FILE *photo;
} bmp_file;
/**
 * Red/Green/Blue color
 */
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
 *          bmp.photo set to NULL when incompatible file.
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
 * @param bmp A bmp photo
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
 * @brief Reveals a hidden photo hidden while still showing the original.
 * @details Alters the original photo by swapping some of its MSbs and LSbs and partially revealing the hidden photo.
 * @param bmp bmp photo containing a hidden photo.
 */
void peek(bmp_file bmp);
/**
 * @brief Hides one photo inside of another photo.
 * @details Stores the MSbs of the hidden photo as the LSbs of the target photo.
 * @param target Target bmp photo which will hide the other photo.
 * @param hidden bmp photo to hide inside of the target photo
 */
void hide(bmp_file target, bmp_file hidden);
/**
 * @brief Invert the pixels of an image.
 * @details Flips the bits of each pixel to create a hue opposite of the original color.
 * @param bmp A bmp photo to be inverted.
 */
void invert(bmp_file bmp);
/**
 * @brief Grayscale an image.
 * @details
 * @param bmp A bmp photo to change to grayscale.
 */
void grayscale(bmp_file bmp);
/**
 * @brief Flip a photo horizontally.
 * @details Flip a photo across the y-axis.
 * @param bmp A bmp photo to flip.
 */
void hflip_image(bmp_file bmp);
/**
 * @brief Mirror a photo down the center copyint the left side to the right.
 * @details Copy the left side of the photo to the right side but horizontally flipped.
 * @param bmp A bmp photo to mirror.
 */
void mirror(bmp_file bmp);

/********************/
/* Bit Manipulation */
/********************/
/**
 * @brief Swap the values of two colors.
 * @param color1 A pointer to the first color.
 * @param color2 A pointer to the second color.
 */
void swap(rgb *color1, rgb *color2);
/**
 * @brief Copies the values of the first color to the second color.
 * @param color1 A pointer to the first color.
 * @param color2 A pointer to the second color.
 */
void copy(rgb *color1, rgb *color2);
/**
 * @brief Swaps the most and least significant bits of a color.
 * @details Swaps the most significant bits with its least significant bits.
 * @param color A single color.
 * @return Returns a color with swapped bits.
 */
char swap_bits(char color);
/**
 * @brief Creates a color storing the most significant bits of both colors in the order of the parameters.
 * @details Stores in a new color the most significant bits of two colors.
 *          Takes the MSbs of color1 as the MSbs of the new color and
 *          the MSbs of color2 as the LSbs of the new color.
 * @param color1 This MSbs of this color are the MSbs of the new color.
 * @param color2 This MSbs of this color are the LSbs of the new color.
 * @return Returns a color with the MSbs of two colors, the MSbs of the new color reflect color1.
 */
char combine_bits(char color1, char color2);
/**
 * @brief Inverts a color by flipping to opposite hue on the color wheel.
 * @details Flips the bits of the given color.
 * @param color A single color.
 * @return Returns an inverted color with the complementing bit pattern.
 */
char invert_bits(char color);

/*****************************/
/* Compression and Expansion */
/*****************************/
/**
 * @brief Linearize the color.
 * @details Linearize using gamma expansion.
 * @param color A single color.
 * @return Returns the linearized sRGB value.
 */
double linearize(char color);
/**
 * @brief Delinearize a color.
 * @details Delinearize using gamma compression the luminance, a weighted sum of the linearized RGB colors.
 * @param color A single color, linearized and in sRGB colorspace.
 * @return Returns the delinearized monochromat value.
 */
char delinearize(double color_lin);

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
void checked_read(void *restrict __ptr, size_t __size, size_t __nitems, FILE *restrict __stream);
/**
 * @brief Performs fwrite and writes to stderr upon failure.
 * @param __ptr Pointer to where the data should be written.
 * @param __size Size of the data.
 * @param __nitems Number of items to be written.
 * @param __stream File stream from which data is being written.
 */
void checked_write(void *restrict __ptr, size_t __size, size_t __nitems, FILE *restrict __stream);

/****************************************/
/************** Validation **************/
/****************************************/
/**
 * @brief Validate that the bits per pixel is 24.
 * @param bmp The bits per pixel of a bmp image.
 */
int validate_bpp(int bpp);