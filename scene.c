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

  if (fscanf(scene->fd, "VP %f %f %f\n", &scene->viewport_size.z,
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

  scene->spheres = malloc(scene->sphere_count * sizeof(sphere_ptr));

  for (int i = 0; i < scene->sphere_count; i++) {
    scene->spheres[i] = *(sphere_ptr)malloc(sizeof(sphere));

    fscanf(scene->fd, "S %f %f %f %hhu %hhu %hhu %hhu\n",
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

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      float dx = (cx / (width - 1)) * i - cx / 2;
      float dy = (cy / (height - 1)) * j - cy / 2;
      float dz = cz;

      float vector_lenght = sqrt(cx * cx + cy * cy + cz * cz);

      vector ray_direction = {cx / vector_lenght, cy / vector_lenght,
                              cz / vector_lenght};

      double closest_intersection = INFINITY;
      sphere closest_sphere;

      for (int i = 0; i < scene->sphere_count; i++) {
        double center_x = scene->spheres[i].center.x;
        double center_y = scene->spheres[i].center.y;
        double center_z = scene->spheres[i].center.z;
        double radius = scene->spheres[i].radius;

        vector sphere_center = {center_x, center_y, center_z};

        double a = dot_product(ray_direction, ray_direction);
        double b = -2 * dot_product(sphere_center, ray_direction);
        double c = dot_product(sphere_center, sphere_center) - radius;

        double discriminant = b * b - 4 * a * c;

        if (discriminant == 0) {
          double d_intersection = fabs(-b / 2 * a);
          if (d_intersection < closest_intersection) {
            closest_intersection = d_intersection;
            closest_sphere = scene->spheres[i];
          }
        }
        if (discriminant > 0) {
          double d_first_intersection = fabs((-b + sqrt(discriminant)) / 2 * a);
          double d_second_intersection =
              fabs((-b - sqrt(discriminant)) / 2 * a);
          if (d_first_intersection < closest_intersection ||
              d_second_intersection < closest_intersection) {
            closest_intersection =
                fmin(d_first_intersection, d_second_intersection);
            closest_sphere = scene->spheres[i];
          }
        }
      }

      image[i*height + width].r = closest_sphere.color.r;
      image[i*height + width].g = closest_sphere.color.g;
      image[i*height + width].b = closest_sphere.color.b;
    }
  }

  return image;
}
