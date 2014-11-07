#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "Mandelbrot.h"
#include "ImageProcess.h"

double getElapsed(struct timeval* t1);


//From Dr. Harris
double getElapsed(struct timeval* t1)
{
     //Calculates Wall time
     double ret;
     struct timeval t2;
     
     gettimeofday(&t2, NULL);
     
     //tv_usec = micro seconds, sec = seconds. 
     ret = t2.tv_usec + t2.tv_sec * 1000000.0;
     ret -= t1->tv_usec + t1->tv_sec * 1000000.0;
     ret /= 1000000.0;

     return ret;
}

int main(int argc, char** argv)
{
     //Height and width of image will be passed in.
     int height = atoi(argv[1]);
     int width = atoi(argv[2]);
     
     Complex num;
     struct timeval start;
     double time = 0.0;
     
     //Mandelbrot Set will have lie in this plane. 
     //X range
     float realMax = 2.0;
     float realMin = -2.0;
     
     //Y range
     float imagMax = 2.0;
     float imagMin = -2.0;
     
     //Scale the image so that it can be seen at the give resolution.
     float scaleX = (realMax - realMin) / width;
     float scaleY = (imagMax - imagMin) / height;

     //Image array
     unsigned int* image 
          = (unsigned int *) malloc(sizeof(unsigned int) * height * width);
     
     //Starting the clock
     gettimeofday(&start, NULL); 
     
     for (int y = 0; y < height; y++)
     {
          for (int x = 0; x < width; x++)
          {
               //Initialize Complex based on position.
               num.real = realMin + ((float) x * scaleX);
               num.imag = imagMin + ((float) y * scaleY);
               
               //Calculates the color of the current pixel.
               image[ y * width + x ] = calPixel(num);
          }
     }
     
     //Stop the clock
     time = getElapsed(&start);
     
     //Output result
     printf("Runtime for sequential code: %dx%d %fs\n", height, width, time);
     
     //Calculate I/O time
     //gettimeofday(&start, NULL);
     
     //Display the set
     //writeImage("seq.ppm", image, height, width); 
     
     //Stop the clock
     //time = getElapsed(&start);
     
     //Output result
     //printf("Runtime for file I/O: %fs\n", time);
  
     free(image);
     return 0;
}


