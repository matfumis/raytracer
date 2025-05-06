#include <stdio.h>
#include "scene.h"

int main(int argc, char *argv[]) { 
  scene scene;
  open_scene_file("test.txt", &scene);
  pixel_ptr image = render_image(&scene, 1920, 1080);
  return 0; 
}