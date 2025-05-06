#include <stdio.h>

struct _ppm_image{
  int width;
  int height;
  int offset;
  int size;
  FILE * fd;
  char * data;
};