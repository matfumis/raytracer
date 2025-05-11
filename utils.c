/*
* Matteo Alessandro Fumis (IN2000249)
*/

#include "scene.h"
#include <math.h>

float inner_product(vector a, vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector normalize_vector(vector v){
  float vector_lenght = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
  vector normalized_vector = {v.x/vector_lenght, v.y/vector_lenght, v.z/vector_lenght};
  return normalized_vector;
}