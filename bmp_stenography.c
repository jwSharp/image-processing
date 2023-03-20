// Author: Jacob Sharp
// University of Pittsburgh: 449 - Project 1

#include <stdio.h>
#include <string.h>


typedef struct {
  char id[2];
  int size;
  short reserved1, reserved2;
  int offset;
} bitmap_file_header;

typedef struct {
  int num_bytes, width, height;
  short num_planes, num_bpp;
  int scheme, size, hres, vres, num_colors, num_imp_colors;
} dib_header;

typedef struct {
  bitmap_file_header bitmap;
  dib_header dib;
} bmp;


void check_read(size_t read, int goal);
void display_header(bmp header);


int main(int argc, char** argv) {
  //
  // Open file
  if (argc < 2) {
    fprintf(stderr, "No file name given.");
    return -1;
  }

  char* filename = argv[1];
  // char* filename2 = argv[2]; // if --hide
  
  FILE* photo = fopen(filename, "r");
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
  check_read(fread(& header.bitmap.size, sizeof(header.bitmap.size), 1, photo), 1);
  check_read(fread(& header.bitmap.reserved1, sizeof(header.bitmap.reserved1), 1, photo), 1);
  check_read(fread(& header.bitmap.reserved2, sizeof(header.bitmap.reserved2), 1, photo), 1);
  check_read(fread(& header.bitmap.offset, sizeof(header.bitmap.offset), 1, photo), 1);

  // Set DIB header values
  check_read(fread(& header.dib.num_bytes, sizeof(header.dib.num_bytes), 1, photo), 1);
  check_read(fread(& header.dib.width, sizeof(header.dib.width), 1, photo), 1);
  check_read(fread(& header.dib.height, sizeof(header.dib.height), 1, photo), 1);
  check_read(fread(& header.dib.num_planes, sizeof(header.dib.num_planes), 1, photo), 1);
  check_read(fread(& header.dib.num_bpp, sizeof(header.dib.num_bpp), 1, photo), 1);
  check_read(fread(& header.dib.scheme, sizeof(header.dib.scheme), 1, photo), 1);
  check_read(fread(& header.dib.size, sizeof(header.dib.size), 1, photo), 1);
  check_read(fread(& header.dib.hres, sizeof(header.dib.hres), 1, photo), 1);
  check_read(fread(& header.dib.vres, sizeof(header.dib.vres), 1, photo), 1);
  check_read(fread(& header.dib.num_colors, sizeof(header.dib.num_colors), 1, photo), 1);
  check_read(fread(& header.dib.num_imp_colors, sizeof(header.dib.num_imp_colors), 1, photo), 1);

  //
  // Display Headers
  display_header(header);

  //
  // Close file
  fclose(photo);

  return 0;
}

void check_read(size_t read, int goal){
  /*
   * Checks for proper number of successfully read objects.
   */
  if ((int) read != goal) {
    if ((int) read > 0) {
      fprintf(stderr, "Not all elements were read.");
    }
    else {
      fprintf(stderr, "No elements were read.");
    }
  }
}

void display_header(bmp header) {
  fprintf(stdout, "=== BMP Header ===\n");
  fprintf(stdout, "Type: %2s\n", header.bitmap.id);
  fprintf(stdout, "Size: %i\n", header.bitmap.size);
  fprintf(stdout, "Reserved 1: %i\n", header.bitmap.reserved1);
  fprintf(stdout, "Reserved 2: %i\n", header.bitmap.reserved2);
  fprintf(stdout, "Image offset: %i\n", header.bitmap.offset);
  fprintf(stdout, "\n=== DIB Header ===\n");
  fprintf(stdout, "Size: %i\n", header.dib.num_bytes);
  fprintf(stdout, "Width: %i\n", header.dib.width);
  fprintf(stdout, "Height: %i\n", header.dib.height);
  fprintf(stdout, "# color planes: %i\n", header.dib.num_planes);
  fprintf(stdout, "# bits per pixel: %i\n", header.dib.num_bpp);
  fprintf(stdout, "Compression scheme: %i\n", header.dib.scheme);
  fprintf(stdout, "Image size: %i\n", header.dib.size);
  fprintf(stdout, "Horizontal resolution: %i\n", header.dib.hres);
  fprintf(stdout, "Vertical resolution: %i\n", header.dib.vres);
  fprintf(stdout, "# colors in palette: %i\n", header.dib.num_colors);
  fprintf(stdout, "# important colors: %i\n", header.dib.num_imp_colors);
}










