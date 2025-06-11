/*
* Matteo Alessandro Fumis (IN2000249)
*/

#include "scene.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/*
Function save_image_as_ppm()

Parameters: 
1. Output image file name
2. Pointer to the rendered image
3. Image width 
4. Image height

Output:
- The function returns an int, 0 if the file is saved with no errors,
1 if an error occurred in the process

The function:
1. Opens the output .ppm file with necessary permissions
2. Computes header and data size, sets the necessary .ppm file size
3. Maps the file to memory
4. Writes header and copies the content of the rendered image 
  (array of pixels) from memory to file
5. Releases recourses

In case of errors while opening .ppm file or mapping, the function returns with an
error code
*/

int save_image_as_ppm(char *filename, pixel_ptr image, int width, int height) {
  FILE *fp = fopen(filename, "w+"); 
  
  if (fp == NULL) {
    return 1;
  }

  // computes the total file size
  int header_size = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
  int data_size = width * height * 3;
  int total_file_size = header_size + data_size;

  // sets the file size
  if (ftruncate(fileno(fp), total_file_size) == -1) {
    fprintf(stderr,"Failed to set file size\n");
    fclose(fp);
    return 1;
  }

  // maps file to memory
  char *data = mmap((void *)0, total_file_size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, fileno(fp), 0);

  if (data == MAP_FAILED) {
    fprintf(stderr,"Failed to map file\n");
    fclose(fp);
    return 1;
  }

  // prints header to file
  snprintf(data, header_size + 1, "P6\n%d %d\n255\n", width, height);

  // copies the array of pixels to the mapped file, starting from the end of the header
  memcpy(data + header_size, image, data_size);

  // File unmapping
  if (munmap(data, total_file_size) == -1) {
    printf("Error while removing mapping\n");
    fclose(fp);
    return 1;
  }

  // File closing
  fclose(fp);
  return 0;
}
