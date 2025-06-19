/*
* Matteo Alessandro Fumis (IN2000249)
*/

#ifndef _SCENE_H
#define _SCENE_H

#include <stdio.h>

struct __attribute__((packed)) _pixel{
  u_int8_t r;
  u_int8_t g;
  u_int8_t b;
}; 

typedef struct _pixel pixel;
typedef struct _pixel * pixel_ptr;

struct _vector{
  float x;
  float y;
  float z;
};

typedef struct _vector vector;
typedef struct _vector * vector_ptr;

struct _sphere {
  vector center;
  float radius;
  pixel color;
};

typedef struct _sphere sphere;
typedef struct _sphere * sphere_ptr;

struct _scene {
  int sphere_count;
  pixel bg_color;
  FILE * fd;
  sphere_ptr spheres;
  vector viewport_size;
};

typedef struct _scene scene;
typedef struct _scene * scene_ptr;

int open_scene_file(char *path, scene_ptr scene);
int render_image(scene_ptr scene, pixel_ptr image, int width, int height);

#endif