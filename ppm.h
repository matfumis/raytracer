/*
* Matteo Alessandro Fumis (IN2000249)
*/

#ifndef _PPM_H
#define _PPM_H

#include <stdio.h>
#include "scene.h"
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int save_image_as_ppm(char *filename, pixel_ptr image, int width, int height);

#endif