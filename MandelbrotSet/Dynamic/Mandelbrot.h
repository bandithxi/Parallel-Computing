#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAXITER 256

//Imaginary numbers, contain two parts. 
typedef struct 
{
     float real;
     float imag;
} Complex;

int calPixel(Complex);

#endif
