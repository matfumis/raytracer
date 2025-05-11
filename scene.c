/*
 * Matteo Alessandro Fumis (IN2000249)
 */

#include "scene.h"
#include "utils.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

int open_scene_file(char *path, scene_ptr scene) {
  scene->fd = fopen(path, "r");
  if (scene->fd == NULL) {
    return -1;
  }

  if (fscanf(scene->fd, "VP %f %f %f\n", &scene->viewport_size.x,
             &scene->viewport_size.y, &scene->viewport_size.z) != 3) {
    fclose(scene->fd);
    return -2;
  }

  if (fscanf(scene->fd, "BG %hhu %hhu %hhu\n", &scene->bg_color.r,
             &scene->bg_color.g, &scene->bg_color.b) != 3) {
    fclose(scene->fd);
    return -3;
  }

  if (fscanf(scene->fd, "OBJ_N %d\n", &scene->sphere_count) != 1) {
    fclose(scene->fd);
    return -4;
  }

  scene->spheres = malloc(scene->sphere_count * sizeof(struct _sphere));

  for (int i = 0; i < scene->sphere_count; i++) {
    fscanf(scene->fd, "S %f %f %f %f %hhu %hhu %hhu\n",
           &(scene->spheres[i].center.x), &(scene->spheres[i].center.y),
           &(scene->spheres[i].center.z), &(scene->spheres[i].radius),
           &(scene->spheres[i].color.r), &(scene->spheres[i].color.g),
           &(scene->spheres[i].color.b));
  }

  return 0;
}

int render_image(scene_ptr scene, pixel_ptr image, int width, int height) {

  for (int i = 0; i < width * height; i++) {
    image[i] = scene->bg_color;
  }

  vector vp = scene->viewport_size;

  #pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      vector ray = {(vp.x / (width - 1)) * i - (vp.x / 2),
                    (vp.y / (height - 1)) * j - (vp.y / 2), vp.z};

      vector ray_direction = normalize_vector(ray);

      float closest_intersection = INFINITY;
      sphere closest_sphere;

      for (int k = 0; k < scene->sphere_count; k++) {
        vector sphere_center = scene->spheres[k].center;
        float radius = scene->spheres[k].radius;

        float a = inner_product(ray_direction, ray_direction);
        float b = -2 * inner_product(sphere_center, ray_direction);
        float c = inner_product(sphere_center, sphere_center) - radius * radius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
          float intersection = fmin(fabs(-b + sqrt(discriminant)) / (2 * a),
                                    fabs(-b - sqrt(discriminant)) / (2 * a));
          if (intersection < closest_intersection) {
            closest_intersection = intersection;
            closest_sphere = scene->spheres[k];
          }
        }

        if (discriminant == 0) {
          float intersection = fabs(-b / (2 * a));
          if (intersection < closest_intersection) {
            closest_intersection = intersection;
            closest_sphere = scene->spheres[k];
          }
        }
      }

      if (closest_intersection < INFINITY) {
        image[(height - 1 - j) * width + i] = closest_sphere.color;
      }
    }
  }

  return 0;
}
