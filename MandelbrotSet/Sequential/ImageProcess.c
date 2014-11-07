#include "ImageProcess.h"
#include <math.h>

/* Writes a Portable PixMap, requires all file name to have ext .ppm to work. 
 */
bool writeImage(char* name, unsigned int* img, int height, int width)
{
     /** This function is a C ported verions of Dr.Bebis' code. 
         It now handles colors.**/
     FILE* fptr;
     
     //Sets write mode to binary
     fptr = fopen(name, "w");
     
     if (!fptr)
     {
          printf("Can't open file: %s", name);
          exit(1);
     }
     
     //PPM in binary mode is P6 
     fprintf(fptr, "P3\n");
     fprintf(fptr, "%d %d\n", width, height);
     fprintf(fptr, "255\n");
     
     for (int idx = 0; idx < height * width; idx++)
     {
          //Convert to RGB
          Color* color = toRGB(img[idx]);
          
          unsigned int red = color->red;
          unsigned int green = color->green;
          unsigned int blue = color->blue;
 
          //Print the RGB
          fprintf(fptr, "%3u %3u %3u", red, green, blue);
          
          //Figure out if need a new line or white space
          int len = idx + 1;
          
          if (len % width == 0) 
          { 
               fprintf(fptr, "\n");
          }
          else 
          {
               fprintf(fptr, "   ");
          }         
     }
 
     fclose(fptr);

     return true;
}

//Pigeon hole the number of iteration into some power of 2
Color* toRGB(unsigned int iter)
{
     Color* out = (Color*) malloc(1);
     
     if (iter == 256)
     {
          out->red = 0;
          out->green = 0;
          out->blue = 0;
          
          return out;
     } 
     
     //Only do these calculation, if we didn't hit MAXITER.
     float num = floor(log2f(iter));
    
     //Log of powers of 2's
     if (num == 1.0)
     {
          out->red = 255;
          out->green = 0;
          out->blue = 0;
     }
     else if (num == 2.0)
     {
          out->red = 255;
          out->green = 127;
          out->blue = 0;
     }
     else if (num == 3.0)
     {
          out->red = 255;
          out->green = 255;
          out->blue = 0;
     }
     else if (num <= 4)
     {
          out->red = 0;
          out->green = 255;
          out->blue = 0;
     }
     else if (num <= 5)
     {
          out->red = 0;
          out->green = 0;
          out->blue = 255;
     }
     else if (num == 6.0)
     {
          
          out->red = 130;
          out->green = 0;
          out->blue = 75;
     }
     else if (num == 7.0)
     {
          out->red = 143;
          out->green = 0;
          out->blue = 255;
     }
     else
     {
          out->red = 256;
          out->green = 256;
          out->blue = 256;
  
     }
     return out;
}
