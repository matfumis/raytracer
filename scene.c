#include "scene.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

double dot_product(vector a, vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

pixel_ptr render_image(scene_ptr scene, int width, int height) {

  pixel_ptr image = malloc(width * height * sizeof(struct _pixel));

  for (int i = 0; i < width * height; i++) {
    image[i].r = scene->bg_color.r;
    image[i].g = scene->bg_color.g;
    image[i].b = scene->bg_color.b;
  }

  float cx = scene->viewport_size.x;
  float cy = scene->viewport_size.y;
  float cz = scene->viewport_size.z;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      float dx = (cx / (width - 1)) * i - (cx / 2);
      float dy = (cy / (height - 1)) * j - (cy / 2);
      float dz = cz;

      float vector_lenght = sqrt(dx * dx + dy * dy + dz * dz);

      vector ray_direction = {dx / vector_lenght, dy / vector_lenght,
                              dz / vector_lenght};

      float closest_intersection = INFINITY;
      sphere closest_sphere = {};

      for (int k = 0; k < scene->sphere_count; k++) {
        float center_x = scene->spheres[k].center.x;
        float center_y = scene->spheres[k].center.y;
        float center_z = scene->spheres[k].center.z;
        float radius = scene->spheres[k].radius;

        vector sphere_center = {center_x, center_y, center_z};

        float a = dot_product(ray_direction, ray_direction);
        float b = -2 * dot_product(sphere_center, ray_direction);
        float c = dot_product(sphere_center, sphere_center) - radius*radius;

        double discriminant = b * b - 4 * a * c;

        if (discriminant == 0) {
          float d_intersection = -b / (2 * a);
          if (d_intersection > 0 && d_intersection < closest_intersection) {
            closest_intersection = d_intersection;
            closest_sphere = scene->spheres[k];
          }
        }
        if (discriminant > 0) {
          float d_first_intersection = (-b + sqrt(discriminant)) / (2 * a);
          float d_second_intersection = (-b - sqrt(discriminant)) / (2 * a);

          if (d_first_intersection > 0 && d_second_intersection > 0) {
            double closest_t =
                fmin(d_first_intersection, d_second_intersection);
            if (closest_t < closest_intersection) {
              closest_intersection = closest_t;
              closest_sphere = scene->spheres[k];
            }
          } else if (d_first_intersection > 0 && d_second_intersection<0) {
            if (d_first_intersection < closest_intersection) {
              closest_intersection = d_first_intersection;
              closest_sphere = scene->spheres[k];
            }
          } else if (d_second_intersection > 0 && d_first_intersection < 0) {
            if (d_second_intersection < closest_intersection) {
              closest_intersection = d_second_intersection;
              closest_sphere = scene->spheres[k];
            }
          }
        }
      }

      if (closest_intersection < INFINITY) {
        image[j * width + i].r = closest_sphere.color.r;
        image[j * width + i].g = closest_sphere.color.g;
        image[j * width + i].b = closest_sphere.color.b;
      }
    }
  }

  return image;
}
