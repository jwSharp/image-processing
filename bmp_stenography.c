// Author: Jacob Sharp
// University of Pittsburgh: 449 - Project 1

#include <stdio.h>

int main(int argc, char** argv) {
  //
  // Open file
  char* filename = "pepper.bmp";
  FILE* photo = fopen(filename, "r");
  if (photo == NULL) {
    fprintf(stderr, "File not successfully opened.");
    return -1;
  }

  //
  // Close file
  fclose(photo);

  return 0;
}
