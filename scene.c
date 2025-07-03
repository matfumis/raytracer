/*
 * Matteo Alessandro Fumis (IN2000249)
 */

#include "scene.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
/*
Function open_scene_file()

Parameters:
1. A scene file path
2. A pointer to a scene struct

Output:
- The function returns an int, 0 if the scene struct is filled with no errors,
1 if an error occurred in the process

The function:
1. Opens the scene file
2. Scans the scene file header assuming its format, filling the scene struct
fields
  - this includes filling as many spheres structs as specified in the scene file

In case of invalid path or malformed scene file, the returns with an error
*/

int read_scene_file(char * scene_file, scene_ptr scene) {

  // opens scene file in read mode only
  scene->fd = fopen(scene_file, "r");
  if (scene->fd == NULL) {
    fprintf(stderr, "Error while opening the scene file: %s\n", scene_file);
    return 1;
  }

  // reads viewport dimensions and fills the correspondent scene field
  if (fscanf(scene->fd, "VP %f %f %f\n", &scene->viewport_size.x,
             &scene->viewport_size.y, &scene->viewport_size.z) != 3) {
    fprintf(stderr, "Malformed viewport header section");
    fclose(scene->fd);
    return 1;
  }

  // reads background color and fills the correspondent scene field
  if (fscanf(scene->fd, "BG %hhu %hhu %hhu\n", &scene->bg_color.r,
             &scene->bg_color.g, &scene->bg_color.b) != 3) {
    fprintf(stderr, "Malformed background color header section");
    fclose(scene->fd);
    return 1;
  }

  // reads sphere count and fills the correspondent scene field
  if (fscanf(scene->fd, "OBJ_N %d\n", &scene->sphere_count) != 1) {
    fprintf(stderr, "Malformed number of objects header section");
    fclose(scene->fd);
    return 1;
  }

  // allocates the memory for an array of sphere structs
  scene->spheres = malloc(scene->sphere_count * sizeof(struct _sphere));
  if (scene->spheres == NULL) {
    fprintf(stderr, "Spheres allocation failed\n");
    return 1;
  }

  /*
  for each sphere in the array, reads it position, radius and color and
  fills the correspondent sphere fields
  */
  for (int i = 0; i < scene->sphere_count; i++) {
    int params_read =
        fscanf(scene->fd, "S %f %f %f %f %hhu %hhu %hhu\n",
               &(scene->spheres[i].center.x), &(scene->spheres[i].center.y),
               &(scene->spheres[i].center.z), &(scene->spheres[i].radius),
               &(scene->spheres[i].color.r), &(scene->spheres[i].color.g),
               &(scene->spheres[i].color.b));

    // Checks if all 7 parameters were read correctly
    if (params_read != 7) {
      fprintf(stderr,
              "Malformed sphere %d definition: expected 7 parameters, got %d\n",
              i + 1, params_read);
      free(scene->spheres);
      fclose(scene->fd);
      return 1;
    }

    // Validates sphere radius
    if (scene->spheres[i].radius <= 0) {
      fprintf(stderr, "Invalid radius for sphere %d: %f (must be positive)\n",
              i + 1, scene->spheres[i].radius);
      free(scene->spheres);
      fclose(scene->fd);
      return 1;
    }

    // Validates color range [0-255]
    if (scene->spheres[i].color.r > 255 || scene->spheres[i].color.g > 255 ||
    scene->spheres[i].color.b > 255 || scene->spheres[i].color.r < 0 || scene->spheres[i].color.g < 0 ||
    scene->spheres[i].color.b < 0) {
      fprintf(stderr, "Color values for spheres must be in range [0-255]\n");
      free(scene->spheres);
      fclose(scene->fd);
      return 1;
    }
  }

  return 0;
}

// Inner product function
float inner_product(vector a, vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Vector normalization function
vector normalize_vector(vector v) {
  float vector_length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  vector normalized_vector = {v.x / vector_length, v.y / vector_length,
                              v.z / vector_length};
  return normalized_vector;
}

/*
Function render_image()

Parameters:
1. A pointer to a scene struct
2. A pointer to an array of pixels
3. Image width in pixels
4. Image height in pixels

Output:
- The function returns an int, 0 if the array of pixels is filled with no errors

The function, FOR EACH PIXEL of the image:
1. Computes the ray direction passing through the viewport
2. Checks, FOR EACH SPHERE in the scene, which one is the
  one the closest to the camera intersected by the ray
3. If an intersection is found, sets the color of the closest
  sphere (the closest intersection) as the color of the pixel.
  If not, sets the default bg color

*/

int render_image(scene_ptr scene, pixel_ptr image, int width, int height) {

  vector vp = scene->viewport_size;

// openmp parallelization
#pragma omp parallel for collapse(2)
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {

      // computation of the vector passing through the viewport at pixel (i,j)
      vector ray = {(vp.x / (width - 1)) * i - (vp.x / 2),
                    (vp.y / (height - 1)) * j - (vp.y / 2), vp.z};

      // direction found by normalizing the vector
      vector ray_direction = normalize_vector(ray);

      // no intersection initially
      float closest_intersection = INFINITY;
      sphere closest_sphere;

      // for each sphere of the scene, computes the intersection, if exists
      for (int k = 0; k < scene->sphere_count; k++) {
        vector sphere_center = scene->spheres[k].center;
        float radius = scene->spheres[k].radius;

        float a = inner_product(ray_direction, ray_direction);
        float b = -2 * inner_product(sphere_center, ray_direction);
        float c = inner_product(sphere_center, sphere_center) - radius * radius;

        float discriminant = b * b - 4 * a * c;

        // double intersection case
        if (discriminant > 0) {
          float intersection = fmin(fabs(-b + sqrt(discriminant)) / (2 * a),
                                    fabs(-b - sqrt(discriminant)) / (2 * a));
          if (intersection < closest_intersection) {
            // overrides closest intersection and closest sphere
            closest_intersection = intersection;
            closest_sphere = scene->spheres[k];
          }
        }

        // single intersection case
        if (discriminant == 0) {
          float intersection = fabs(-b / (2 * a));
          if (intersection < closest_intersection) {
            // overrides closest intersection and closest sphere
            closest_intersection = intersection;
            closest_sphere = scene->spheres[k];
          }
        }
      }

      /*
      if there is an actual intersection,
      colors the pixel as the closest intersected sphere,
      if not, colors the pixel as background
      */
      if (closest_intersection < INFINITY) {
        image[(height - 1 - j) * width + i] = closest_sphere.color;
      } else {
        image[(height - 1 - j) * width + i] = scene->bg_color;
      }
    }
  }
  return 0;
}
