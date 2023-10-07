/**
 * @file main.c
 * @author Jacob Sharp
 * @copyright MIT License
 */

#include <stdio.h>
#include <string.h>
#include "stenography.h"

bmp_file prompt_photo(char *prompt);

int main(int argc, char **argv)
{
    printf("Welcome to image stenography.\n");
    printf("Please select from the options below by typing the number of the operation you wish to perform:\n");

    bmp_file bmp, host, hidden;
    int choice;
    int flag = 1;
    while (flag)
    {
        printf("\n1. Display Header\n");
        printf("2. Reveal Photo\n");
        printf("3. Hide Photo\n");
        printf("4. Invert Photo\n");
        printf("5. Grayscale Photo\n");
        printf("6. Flip Photo\n");
        printf("7. Mirror Photo\n");
        printf("Your Response:\t");

        scanf("%1d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of a bmp file located in the images folder.\n");

            // display the bmp's header
            display_header(bmp);

            // close file
            close_bmp(bmp);
            break;

        case 2:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // reveal the hidden photo
            reveal(bmp);

            // close file
            close_bmp(bmp);
            break;

        case 3:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // show both the hidden and original photo
            peek(bmp);

            // close file
            close_bmp(bmp);
            break;

        case 4:
            // open host and hidden photos
            host = prompt_photo("Enter the filepath of the host bmp file which will hold the hidden photo.\n");
            hidden = prompt_photo("Enter the filepath of the bmp file that will be hidden.\n");

            // hide the photo
            hide(host, hidden);

            // close files
            close_bmp(host);
            close_bmp(hidden);
            break;

        case 5:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // invert the photo
            invert(bmp);

            // close files
            close_bmp(bmp);
            break;

        case 6:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // turn the photo to grayscale
            grayscale(bmp);

            // horizontally flip the photo
            hflip_image(bmp);

            close_bmp(bmp);
            break;

        case 7:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // horizontally flip the photo
            hflip_image(bmp);

            close_bmp(bmp);
            break;

        case 8:
            // prompt for bmp file
            bmp = prompt_photo("Enter the filepath of the bmp file.\n");

            // mirror the photo down the center
            mirror(bmp);

            close_bmp(bmp);
            break;

        default:
            printf("This is an invalid option.\n");
            break;
        }

        // prompt for another operation
        printf("Would you like to perform another operation?\n");
        printf("Type 1 if you wish to do another operation or 0 if not.\n");
        scanf("%1d", &flag);
        if (flag != 1)
        {
            flag = 0;
        }
    }

    printf("\n\nThank you for using image stenography.");
    return 0;
}

bmp_file prompt_photo(char *prompt)
{
    bmp_file bmp;
    char image_path = "images/"; // Directory containing images
    char filename[100];
    char filepath[100 + sizeof(image_path)];

    // Prompt until successful file opened
    int success = 0;
    while (!success)
    {
        // Prompt for filename
        printf("%s", prompt);
        scanf("%s", &filename);

        // // Prepend image path
        // if (strncmp(filename, image_path, strlen(image_path)) != 0)
        // {
        //     // Prepend path
        //     snprintf(filepath, sizeof(filepath), "%s%s", image_path, filename);
        // }
        // else
        // {
        //     // Keep filepath
        //     snprintf(filepath, sizeof(filepath), "%s", filename);
        // }

        // open the photo
        bmp = open_bmp(filename);
        if (bmp.photo == NULL)
        {
            fprintf(stderr, "BMP file %s could not be properly opened.\n", filename);
            printf("Photo cannot be opened.\n");
            continue;
        }

        success = 1;
    }

    return bmp;
}