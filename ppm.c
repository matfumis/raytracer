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
4. Image heigth

It does:
1. Open the output .ppm file with necessary permissions
2. Compute header and data size, sets the necessary .ppm file size
3. Map the file to memory
4. Write header and copies the content of the rendered image 
  (array of pixels) to to the file
5. Close the .ppm file

In case of errors while opening .ppm file or mapping, the function returns with an
error code
*/

int save_image_as_ppm(char *filename, pixel_ptr image, int width, int height) {
  int image_file = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);

  if (image_file <= 0) {
    return 1;
  }

  int header_size = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
  int data_size = width * height * 3;
  int total_file_size = header_size + data_size;

  if (ftruncate(image_file, total_file_size) == -1) {
    fprintf(stderr,"Failed to set file size");
    close(image_file);
    return 1;
  }

  char *data = mmap((void *)0, total_file_size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, image_file, 0);

  if (data == MAP_FAILED) {
    fprintf(stderr,"Failed to map file");
    close(image_file);
    return 1;
  }

  snprintf(data, header_size + 1, "P6\n%d %d\n255\n", width, height);

  memcpy(data + header_size, image, data_size);

  if (munmap(data, total_file_size) == -1) {
    printf("Error while removing mapping");
    close(image_file);
    return 1;
  }

  close(image_file);
  return 0;
}
