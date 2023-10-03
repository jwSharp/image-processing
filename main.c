/**
 * @file main.c
 * @author Jacob Sharp
 * @copyright MIT License
 */

#include <stdio.h>
#include <string.h>
#include "stenography.h"

/**
 * @param argv[1] function to run
 */
int main(int argc, char **argv)
{
  // Open file
  if (argc < 2)
  {
    fprintf(stderr, "No file name given.");
    return -1;
  }
  bmp_file bmp = create_bmp(argv[1]);

  // Check photo properly stored
  if (bmp.photo == NULL)
  {
    fprintf(stderr, "BMP file could not be properly opened.");
    printf("Photo cannot be opened.");
    return -1;
  }

  // Display Headers
  display_header(bmp.header);

  // Reveal Hidden File
  reveal(bmp);

  // Close file
  fclose(bmp.photo);

  return 0;
}