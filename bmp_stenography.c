// Author: Jacob Sharp
// University of Pittsburgh: 449 - Project 1

#include <stdio.h>
#include <string.h>


typedef struct {
  char id[2];
  int file_size;
  short reserved1, reserved2;
  int offset;
} bitmap_file_header;

typedef struct {
  int header_size, width, height;
  short planes, bpp;
  int scheme, img_size, hres, vres, num_colors, num_imp_colors;
} dib_header;

typedef struct {
  bitmap_file_header bitmap;
  dib_header dib;
} bmp;


void check_read(size_t read, int goal);
void display_header(bmp header);
void reveal(FILE* photo, bmp header);
char swap_bits(char color);

/********************************************************************************/

int main(int argc, char** argv) {
  //
  // Open file
  if (argc < 2) {
    fprintf(stderr, "No file name given.");
    return -1;
  }

  FILE* photo = fopen(argv[1], "r+");
  if (photo == NULL) {
    fprintf(stderr, "File not successfully opened.");
    return -1;
  }

  //
  // Read contents
  bmp header;
  
  // Set bitmap file header values
  check_read(fread(header.bitmap.id, sizeof(header.bitmap.id), 1, photo), 1);

  if (strncmp(header.bitmap.id, "BM", 2)) {
    fprintf(stderr, "Incorrect file type.");
    return -1;
  }
  check_read(fread(& header.bitmap.file_size, sizeof(header.bitmap.file_size), 1, photo), 1);
  check_read(fread(& header.bitmap.reserved1, sizeof(header.bitmap.reserved1), 1, photo), 1);
  check_read(fread(& header.bitmap.reserved2, sizeof(header.bitmap.reserved2), 1, photo), 1);
  check_read(fread(& header.bitmap.offset, sizeof(header.bitmap.offset), 1, photo), 1);

  // Set DIB header values
  check_read(fread(& header.dib.header_size, sizeof(header.dib.header_size), 1, photo), 1);
  check_read(fread(& header.dib.width, sizeof(header.dib.width), 1, photo), 1);
  check_read(fread(& header.dib.height, sizeof(header.dib.height), 1, photo), 1);
  check_read(fread(& header.dib.planes, sizeof(header.dib.planes), 1, photo), 1);
  check_read(fread(& header.dib.bpp, sizeof(header.dib.bpp), 1, photo), 1);
  check_read(fread(& header.dib.scheme, sizeof(header.dib.scheme), 1, photo), 1);
  check_read(fread(& header.dib.img_size, sizeof(header.dib.img_size), 1, photo), 1);
  check_read(fread(& header.dib.hres, sizeof(header.dib.hres), 1, photo), 1);
  check_read(fread(& header.dib.vres, sizeof(header.dib.vres), 1, photo), 1);
  check_read(fread(& header.dib.num_colors, sizeof(header.dib.num_colors), 1, photo), 1);
  check_read(fread(& header.dib.num_imp_colors, sizeof(header.dib.num_imp_colors), 1, photo), 1);

  //
  // Display Headers
  display_header(header);

  //
  // Reveal Hidden File
  reveal(photo, header);

  //
  // Close file
  fclose(photo);

  return 0;
}


/********************************************************************************/


void check_read(size_t read, int goal){
  /*
   * Checks for proper number of successfully read objects.
   */

  if ((int) read != goal) {
    if ((int) read > 0) {
      fprintf(stderr, "Not all elements were read.");
    } else {
      fprintf(stderr, "No elements were read.");
    }
  }
}

void check_seek(int failure, int origin, long offset) {
  if (failure) {
    if (offset >= 0) {
      fprintf(stderr, "Failed to seek from %i forward %ld", origin, offset);
    } else {
      fprintf(stderr, "Failed to seek from %i backwards %ld", origin, offset);
    }
  }
}


/********************************************************************************/


void display_header(bmp header) {
  fprintf(stdout, "=== BMP Header ===\n");
  fprintf(stdout, "Type: %.2s\n", header.bitmap.id);
  fprintf(stdout, "Size: %i\n", header.bitmap.file_size);
  fprintf(stdout, "Reserved 1: %i\n", header.bitmap.reserved1);
  fprintf(stdout, "Reserved 2: %i\n", header.bitmap.reserved2);
  fprintf(stdout, "Image offset: %i\n", header.bitmap.offset);

  fprintf(stdout, "\n=== DIB Header ===\n");
  fprintf(stdout, "Size: %i\n", header.dib.header_size);
  fprintf(stdout, "Width: %i\n", header.dib.width);
  fprintf(stdout, "Height: %i\n", header.dib.height);
  fprintf(stdout, "# color planes: %i\n", header.dib.planes);
  fprintf(stdout, "# bits per pixel: %i\n", header.dib.bpp);
  fprintf(stdout, "Compression scheme: %i\n", header.dib.scheme);
  fprintf(stdout, "Image size: %i\n", header.dib.img_size);
  fprintf(stdout, "Horizontal resolution: %i\n", header.dib.hres);
  fprintf(stdout, "Vertical resolution: %i\n", header.dib.vres);
  fprintf(stdout, "# colors in palette: %i\n", header.dib.num_colors);
  fprintf(stdout, "# important colors: %i\n", header.dib.num_imp_colors);
}

void reveal(FILE* photo, bmp header) {
  /*
   * Switches each color's MSBs with the LSBs.
   */

  // Determine RGB Format
  if (header.dib.bpp != 24) {
    fprintf(stderr, "Program does not handle alternate color densities. Image must be 24 bpp.");
    fprintf(stdout,"Photo not revealed.");
    return;
  }
  typedef struct {
    char r, g, b;
  } rgb;


  //
  // Alter Colors
  check_seek(fseek(photo, header.bitmap.offset, SEEK_SET), 0, header.bitmap.offset); // from beginning of file
  rgb color;
  int padding = (sizeof(color) * header.dib.width) % 4; // Padding, align row to multiple of 4

  for (int h=0; h<header.dib.height; h++) {
    for (int w=0; w<header.dib.width; w++) {
      check_read(fread(&color, sizeof(color.r), 3, photo), 3);
      rgb before = color;

      // Swap Bits
      color.r = swap_bits(color.r);
      color.g = swap_bits(color.g);
      color.b = swap_bits(color.b);

      // Update photo
      check_seek(fseek(photo, -sizeof(color), SEEK_CUR), SEEK_CUR, -sizeof(color.r) * 3);
      fwrite(&color, sizeof(color), 1, photo);
 
      
      check_seek(fseek(photo, -sizeof(color), SEEK_CUR), SEEK_CUR, -sizeof(color.r) * 3);
      check_read(fread(&color, sizeof(color.r), 3, photo), 3);
      rgb after = color;
      int is_same; if (before.r == after.r) { is_same = 1; } else { is_same = 0; }
      fprintf(stderr, "%i", is_same);
    }
    
    check_seek(fseek(photo, padding, SEEK_CUR), SEEK_CUR, padding); // End of row padding
  }
}


/********************************************************************************/


char swap_bits(char color) {
  /*
   * Use masks to swap the first 4 bits with the last 4 bits.
   */
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

char combine_bits(char color1, char color2) {
  /*
   * Use masks to combine the MSBs of two different photos.
   */
  char msb, lsb;

  // capture MSB of each color
  msb = 0xF0 & color1;
  lsb = 0xF0 & color2;

  // combine colors
  lsb = lsb >> 4;
  return msb | lsb;
}

