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
  printf("Welcome to image stenography.\n");
  printf("Please select from the options below by typing the number of the operation you wish to perform:\n");

  char filename[100], fhost[100], fhidden[100];
  bmp_file bmp, host, hidden;
  int choice;

  typedef int bool;
  bool flag = 1;
  while (flag)
  {
    printf("\n1. Display Header\n");
    printf("2. Reveal Photo\n");
    printf("3. Hide Photo\n");
    printf("4. Invert Photo\n");
    printf("Your Response:\t");

    scanf("%d", &choice);
    printf("\n");

    switch (choice)
    {
    case 1:
      // prompt for bmp file
      printf("Enter the filename of the bmp file.\n");
      scanf("%s", &filename);

      // open the photo
      bmp = open_bmp(filename);
      if (bmp.photo == NULL)
      {
        fprintf(stderr, "BMP file %s could not be properly opened.\n", filename);
        printf("Photo cannot be opened.\n");
        continue;
      }

      display_header(bmp);

      close_bmp(bmp);
      break;

    case 2:
      // prompt for bmp file
      printf("Enter the filename of the bmp file.\n");
      scanf("%s", &filename);

      // open the photo
      bmp = open_bmp(filename);
      if (bmp.photo == NULL)
      {
        fprintf(stderr, "BMP file %s could not be properly opened.\n", filename);
        printf("Photo cannot be opened.\n");
        continue;
      }

      // reveal the hidden photo
      reveal(bmp);

      close_bmp(bmp);
      break;

    case 3:
      // open host photo
      printf("Enter the filename of the host bmp file which will hold the hidden photo.\n");
      scanf("%s", &fhost);
      host = open_bmp(fhost);
      if (host.photo == NULL)
      {
        fprintf(stderr, "BMP file %s could not be properly opened.\n", fhost);
        printf("Photo cannot be opened.\n");
        continue;
      }

      // open hidden photo
      printf("Enter the filename of the bmp file that will be hidden.\n");
      scanf("%s", &fhidden);
      hidden = open_bmp(fhidden);
      if (hidden.photo == NULL)
      {
        fprintf(stderr, "BMP file %s could not be properly opened.\n", fhidden);
        printf("Photo cannot be opened.\n");
        continue;
      }

      // hide the photo
      hide(host, hidden);

      close_bmp(host);
      close_bmp(hidden);
      break;

    case 4:
      // prompt for bmp file
      printf("Enter the filename of the bmp file.\n");
      scanf("%s", &filename);

      // open the photo
      bmp = open_bmp(filename);
      if (bmp.photo == NULL)
      {
        fprintf(stderr, "BMP file %s could not be properly opened.\n", filename);
        printf("Photo cannot be opened.\n");
        continue;
      }

      // invert the photo
      invert(bmp);

      close_bmp(bmp);
      break;

    default:
      printf("This is an invalid option.\n");
      break;
    }

    // prompt for another operation
    printf("Would you like to perform another operation?\n");
    printf("Type 1 if you wish to do another operation or 0 if not.\n");
    scanf("%d", &flag);
    if (flag != 1)
    {
      flag = 0;
    }
  }

  printf("\n\nThank you for using image stenography.");
  return 0;
}