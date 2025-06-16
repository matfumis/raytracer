# Raytracer

---
## Overview

This project consists in the implementation in C of a simple 
raytracer. Given a properly formatted `.txt` file
representing a scene containing a given number of coloured 
spheres, the program is able to render the correspondent image 
in the `.ppm` format. An example of the `.txt` is given in the project 
files. 
---

todo structure
## Structure


## Usage

### Compiling

The program can be compiled by simply typing `make`. For cleaning
up executables and object files use `make clean`.

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