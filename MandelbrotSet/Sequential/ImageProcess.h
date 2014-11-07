#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

//RGB color type
typedef struct 
{
     //Byte type not available to hold 256 values.
     unsigned int red;
     unsigned int green;
     unsigned int blue;
} Color;

Color* toRGB(unsigned int hue);
bool writeImage(char* name, unsigned int* img, int height, int width);

#endif
