/*
 * Matteo Alessandro Fumis (IN2000249)
 */

#include "ppm.h"
#include "scene.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char *argv[]) {

  int width = 1920;
  int height = 1080;
  char *scene_filename = "test.txt";
  char *output_filename = "image.ppm";

  if (argc > 1)
    scene_filename = argv[1];
  if (argc > 2)
    output_filename = argv[2];
  if (argc > 3)
    width = atoi(argv[3]);
  if (argc > 4)
    height = atoi(argv[4]);

  printf("Scene: %s\n", scene_filename);
  printf("Output: %s\n", output_filename);
  printf("Resolution: %d x %d\n", width, height);

  double start = omp_get_wtime();

  scene scene;
  if (open_scene_file(scene_filename, &scene) != 0) {
    fprintf(stderr, "Error opening scene file: %s\n", scene_filename);
    return 1;
  }

  pixel_ptr image = malloc(width * height * sizeof(struct _pixel));
  if (image == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  if (render_image(&scene, image, width, height) != 0) {
    fprintf(stderr, "Error rendering image\n");
    free(image);
    return 1;
  }

  if (save_image_as_ppm(output_filename, image, width, height) != 0) {
    fprintf(stderr, "Error saving image\n");
    free(image);
    return 1;
  }
  free(image);
  double end = omp_get_wtime();
  printf("Wall time: %f seconds\n", end - start);

  return 0;
}