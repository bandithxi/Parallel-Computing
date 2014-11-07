#include "Mandelbrot.h"

//From book
int calPixel(Complex in)
{
      int count;
      Complex curZ; 
      float temp, lengthsq;
      
      curZ.real = 0.0;
      curZ.imag = 0.0;
      count = 0;

      do 
      {
            //z^2 = a^2 - b^2 + 2abi 
            temp = curZ.real * curZ.real - curZ.imag * curZ.imag + in.real;
            curZ.imag = 2 * curZ.real * curZ.imag + in.imag;
            
            //update
            curZ.real = temp;
            
            //file the square of the length for performance
            lengthsq = curZ.real * curZ.real + curZ.imag * curZ.imag;
            count++;

      } while ((lengthsq < 4.0) && (count < MAXITER));

      return count;
}


