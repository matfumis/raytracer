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

/*
Function main()

Parameters: Nothing or
1. Scene file name
2. Output image file name
3. Image width 
4. Image height

The function:
1. Sets default values to required parameters, which can be overwritten
  by the user when calling ./main
2. Instantiates and populate a scene struct from a given scene file
3. Allocates the necessary memory for an image of size passed as parameter
  (width and height) and renders its content from the scene populated at
  point 2
4. Saves the rendered image as ppm file

Execution time is displayed to the user. Allocated resources are freed in case
of error or successful execution.
*/

int main(int argc, char *argv[]) {

  // rises an error if number of parameters is wrong
  if (argc != 5 && argc != 1) {
    fprintf(stderr, "Error while reading parameters. Use the defaults by calling main\n"
                    "./main"
                    "or follow the format:\n"
                    "./main <scene-file.txt> <image-name.ppm> <image-width> <image-height>\n");
    return 1;
  }

  // default parameter values
  char *scene_filename = "test.txt";
  char *output_filename = "image.ppm";
  int width = 1920;
  int height = 1080;

  // User can overwrite parameters
  if (argc == 5) {
    scene_filename = argv[1];
    output_filename = argv[2];
    width = atoi(argv[3]);
    height = atoi(argv[4]);
  }

  printf("Scene: %s\n", scene_filename);
  printf("Output: %s\n", output_filename);
  printf("Resolution: %d x %d\n", width, height);

  float start = omp_get_wtime();

  // opens scene file
  scene scene;
  if (open_scene_file(scene_filename, &scene) != 0) {
    fprintf(stderr, "Errore while opening the scene file: %s\n",
            scene_filename);
    return 1;
  }

  // allocates memory for an array of pixels of given length
  pixel_ptr image = malloc(width * height * sizeof(struct _pixel));
  if (image == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  // renders the image
  if (render_image(&scene, image, width, height) != 0) {
    fprintf(stderr, "Error rendering image\n");
    free(image);
    return 1;
  }

  // saves the image
  if (save_image_as_ppm(output_filename, image, width, height) != 0) {
    fprintf(stderr, "Error saving image\n");
    free(image);
    return 1;
  }

  free(image);
  double end = omp_get_wtime();
  printf("Execution time: %f seconds\n", end - start);

  return 0;
}