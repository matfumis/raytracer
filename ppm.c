#include "scene.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int save_image_as_ppm(char *filename, pixel_ptr image, int width, int height) {
  int img_f = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);

  if (img_f <= 0) {
    return -1;
  }

  int header_size = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
  int data_size = width * height * 3;
  int total_file_size = header_size + data_size;

  if (ftruncate(img_f, total_file_size) == -1) {
    perror("Failed to set file size");
    close(img_f);
    return -1;
  }

  char *data = mmap((void *)0, total_file_size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, img_f, 0);

  if (data == MAP_FAILED) {
    perror("Failed to map file");
    close(img_f);
    return -1;
  }

  snprintf(data, header_size + 1, "P6\n%d %d\n255\n", width, height);

  memcpy(data + header_size, image, data_size);

  if (munmap(data, total_file_size) == -1) {
    printf("Errore nel rimuovere il mapping");
    close(img_f);
    return -1;
  }

  close(img_f);
  return 0;
}
