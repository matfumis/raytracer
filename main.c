#include "ppm.h"
#include "scene.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  scene scene;
  open_scene_file("test.txt", &scene);
  pixel_ptr image = render_image(&scene, 1920, 1080);

  /*
  printf("Viewport: %f, %f, %f\n", scene.viewport_size.x, scene.viewport_size.y,
         scene.viewport_size.z);
  printf("Number of spheres: %d\n", scene.sphere_count);
  for (int i = 0; i < scene.sphere_count; i++) {
    printf("Center, x:%f, y:%f, z:%f, Radius:%f\n", scene.spheres[i].center.x,
           scene.spheres[i].center.y, scene.spheres[i].center.z,
           scene.spheres[i].radius);
    printf("r: %d, g: %d, b: %d\n", scene.spheres[i].color.r,
           scene.spheres[i].color.g, scene.spheres[i].color.b);
  }
           */

  

  save_image_as_ppm("image.ppm", image, 1920, 1080);

  /*
  for (int i = 0; i < 1920 * 1080; i++) {
    if (image[i].r != 0 || image[i].g != 0 || image[i].b != 0) {
      printf("r: %d, g: %d, b: %d\n", image[i].r, image[i].g, image[i].b);
    }
  }
    */
  return 0;
}