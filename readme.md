# Raytracer

---
## Overview

This project consists in the implementation of a simple 
raytracer written in C. Given a properly formatted .txt file
representing a scene containing a given number of coloured 
spheres, the program is able to render the correspondent image 
in the .ppm format.
---

## Usage

### Compiling

The program can be compiled by simply typing `make`. For cleaning
up executables and object files, `make clean` can be used.

### Running

The program is CLI based and can be run with default parameters 
by typing `./main`. The default parameters are:
- Scene text file: `test.txt` (already in the project directory)
- Output: `image.ppm`
- Image width in pixels: `1920`
- Image height in pixels: `1080`

Alternatively, the user can specify custom parameter values by
following the following format:

`./main <scene-file.txt> <image-file.ppm> <pixel-width> <pixel-height>`