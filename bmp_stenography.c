// Author: Jacob Sharp
// University of Pittsburgh: 449 - Project 1

#include <stdio.h>

typedef struct {
  char* id[2];
  int size, reserved1, reserved2;
  void* offset;
} bitmap_file_header;

typedef struct {
  int num_bytes, width, height;
  short num_planes, num_bpp;
  int scheme, size, h_res, v_res, num_colors, num_imp_colors;
} dib_header;

typedef struct {
  bitmap_file_header bitmap;
  dib_header dib;
} bmp;

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
  void* content[100];
  bmp file;

  //
  // Close file
  fclose(photo);

  return 0;
}
