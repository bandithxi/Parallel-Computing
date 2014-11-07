#include <mpi.h>
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
     
     //Number of CPUs
     int numProcs;
     //Processor ID
     int rank;
     
     //The status of our receiver
     MPI_Status status;
     
     //Init MPI, Starts the parallelization sort of. 
     MPI_Init(&argc, &argv);     
     
     //Finds out how many CPUs are in our network
     MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

     //Determines the rank of a process
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     
     //Number of slaves
     int numGroups = numProcs - 1;
     
     //Number of remaining rows after even partitions for slave.
     int remainder = height % numGroups;
     
     //How height those partitions are.
     int grpHeight = (height - remainder) / numGroups;
     
     //The area of our partition
     int partArea = grpHeight * width;
    
     //Image array
     unsigned int* image 
               = (unsigned int *) malloc(sizeof(unsigned int) * height * width);

     unsigned int* temp 
                    = (unsigned int *) malloc(sizeof(unsigned int) * (width + 10));

     MPI_Barrier(MPI_COMM_WORLD);

     if (rank == 0)
     {
          

          //Starting the clock
          gettimeofday(&start, NULL); 
          
          //Receive a batch of pixels from slave inorder
          for (int idx = 0; idx < height - remainder; idx++)
          {    
                         
               //The row we are wroking on
               int row;
               MPI_Recv(&row, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
               
               /* Partition start address 
                  is the array address offset by the elements skipped.
                */ 
               MPI_Recv(temp, 
                        width, 
                        MPI_UNSIGNED, 
                        MPI_ANY_SOURCE, 
                        0, 
                        MPI_COMM_WORLD, 
                        &status);
          
              for (int x = 0; x < width; x++)
              {
                   image[row * width + x] = temp[x];
              }
          }

          //Calculate for the remainding pixels
          for (int y = height - remainder; y < height; y++)
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
          printf("%d cores %dx%d: %fs\n", numProcs, height, width, time);
     
          //Calculate I/O time
          //gettimeofday(&start, NULL);
     
          //Display the set
          //writeImage("Static.ppm", image, height, width); 
     
          //Stop the clock
         // time = getElapsed(&start);
     
          //Output result
          //printf("Runtime for file I/O: %fs\n", time);
                   
     }
     else 
     {
                    
          /* The start location of our partition 
             depends on how big our partitions and how many we skipped.
           */
          int start = (int) (rank - 1) * grpHeight;

          for (int y = start; y < start + grpHeight; y++)
          {
              for (int x = 0; x < width; x++)
              {
                    //Initialize Complex based on position.
                    num.real = realMin + ((float) x * scaleX);
                    num.imag = imagMin + ((float) y * scaleY);
             
                    //Calculates the color of the current pixel.
                    temp[x] = calPixel(num);
               }
               
               MPI_Send(&y, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
         
               //Send only partition worked on
               MPI_Send(temp, 
                        width, 
                        MPI_UNSIGNED, 
                        0, 
                        0, 
                        MPI_COMM_WORLD);

         }
     }
     MPI_Finalize();
     return 0;
}



