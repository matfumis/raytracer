# Raytracer
![](assets/result.jpg)
## Overview

This project consists in the implementation in C of a simple 
raytracer. Given a properly formatted `.txt` file
representing a scene containing a declared number of coloured 
spheres, the program is able to render the correspondent image 
in `.ppm` format. An example of the `.txt` is given in the project 
files. 

---


## Structure

The project is structured in the following files:

- `scene.c`: responsible for scene file reading and scene rendering
- `scene.h`: contains function declarations and auxiliary struct definitions
- `ppm.c`: responsible for saving the rendered image in `.ppm` format
- `ppm.h`: contains function declarations
- `main.c`: responsible for user input parsing and calls to core functions

---

## Usage

### Compiling

The program can be compiled with `make` command. For cleaning
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