#include <mpi.h>
#include "matrix.h"
#include <sys/time.h>

void master(int size);
void slave(int rank, int size);
double getElapsed(struct timeval* t1);

int main(int argc, char** argv)
{
     int size = atoi(argv[1]);
     int rank;
     
     //Initialize MPI
     MPI_Init(&argc, &argv);
     
     //Find the current rank 
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     if (rank == 0)
     {
          master(size);     
     }
     else
     {
          slave(rank, size);
     }

     //Close MPI
     MPI_Finalize();
     return 0;
}



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

void master(int size)
{
     //Number of process, number of rows, start and stop rows, remainding row
     int numProcs, count, start, stop, remainder;
     
     //Averages
     double avgTotal = 0.0, avgCT = 0.0, seqTime = 0.0;
     
     //How long calculation will take
     double wallTime = 0.0;
    
     //The status of our receiver
     MPI_Status status;

     int testRuns = 10;
     struct timeval startTT, startCT;

     //Allocation of matrix
     int* matA = allocMat(size);
     int* matB = allocMat(size);
     int* matC = allocMat(size);
     
     MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
     
     //This test 10 times
    // for (int i = 0; i < testRuns; i++)
  //   {
          
          double totalTime = 0.0, calcTime = 0.0, remTime = 0.0;  
 
          initMatrix(matA, matB, size);

          //Start total time 
          gettimeofday(&startTT, NULL);

          //Broadcast matrix A
          MPI_Barrier(MPI_COMM_WORLD);
          MPI_Bcast(matA, size * size, MPI_INT, 0, MPI_COMM_WORLD);
   
          //Broadcast matrix B
          MPI_Barrier(MPI_COMM_WORLD);
          MPI_Bcast(matB, size * size, MPI_INT, 0, MPI_COMM_WORLD);
     
          start = 0;
          // rows/thread
          stop = size / numProcs;
          //area/thread
          count = size * (size / numProcs);
          
          //Start calculation timer 
          gettimeofday(&startCT, NULL);

          subMatrixCal(matA, matB, matC, size, start, stop);
     
          //Stop calculation timer
          calcTime = getElapsed(&startCT);

          MPI_Gather(matC, count, MPI_INT, matC, count, MPI_INT, 0, MPI_COMM_WORLD);
          
          //Wall time = the slowest computation time
          for (int idx = 1; idx < numProcs; idx++)
          {
               MPI_Recv(&wallTime, 1, MPI_DOUBLE, idx, 0, MPI_COMM_WORLD, &status);
               
               if (wallTime > calcTime)
               {
                    calcTime = wallTime;
               }
              
          }

          remainder = size % numProcs;
          
          if (remainder > 0)
          {    
               //Start remainder timer
               gettimeofday(&startCT, NULL);
               
               remMatrixCal(matA, matB, matC, size, remainder);
               
               //Stop remainder timer
               remTime = getElapsed(&startCT);
          }

          //Add remainder time and calculation time
          calcTime += remTime;

          //Stop total time
          totalTime = getElapsed(&startTT);
     
          //Running total for average total and cal time
          avgTotal += totalTime;
          avgCT += calcTime;
         
   //  }
     
     //Average total and cal time
     avgTotal /= testRuns;
     avgCT /= testRuns;
     
     //Start sequential timer
     gettimeofday(&startCT, NULL);

     calMatrix(matA, matB, size);
     
     //Stop sequential timer
     seqTime = getElapsed(&startCT);
    
     //Find speed up and efficiency
     double speedUp = seqTime / avgCT;
     double efficiency = speedUp / numProcs;
 
     //Print stats
     printf("Size: %d x %d\n", size, size);
     printf("Speed up: %f\n", speedUp);
     printf("Efficiency: %f\n", efficiency);
     printf("Average Total Time: %f s\n", avgTotal);
     printf("Average Sequential time: %f s\n", seqTime);
     printf("Average Parallel time: %f s\n", avgCT);
     printf("Average Communication time: %f s\n\n", avgTotal - avgCT);

     deleteMat(matA, size);
     deleteMat(matB, size);
     deleteMat(matC, size);    

}

void slave(int rank, int size)
{
     int numProcs, count, start, stop;
     int testRuns = 10;
     
     //How long calculation will take
     double wallTime = 0.0;
     
     //Dynamic allocation of arrays
     int* matA = allocMat(size);
     int* matB = allocMat(size);
     int* matC = allocMat(size);
      
     MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
     
     //Rum this test 10 times
    // for (int i = 0; i < testRuns; i++)
     //{ 
          //Broadcast matrix A
          MPI_Barrier(MPI_COMM_WORLD);
          MPI_Bcast(matA, size * size, MPI_INT, 0, MPI_COMM_WORLD);
   
          //Broadcast matrix B
          MPI_Barrier(MPI_COMM_WORLD);
          MPI_Bcast(matB, size * size, MPI_INT, 0, MPI_COMM_WORLD);
     
          //Calculate which part of matrix A is to be used
          start = rank * (size / numProcs);
          stop = (rank + 1) * (size / numProcs);
          count = size * (size / numProcs);

          //Calculate part of matrix C 
          subMatrixCal(matA, matB, matC, size, start, stop);
  
          //Send part of matrix C back to Master
          MPI_Gather(matC, count, MPI_INT, matC, count, MPI_INT, 0, MPI_COMM_WORLD);
          
          //Send the calculation time
          MPI_Send(&wallTime, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
          
   //  }

     deleteMat(matA, size);
     deleteMat(matB, size);
     deleteMat(matC, size);    
}
