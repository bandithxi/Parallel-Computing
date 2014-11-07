#include "matrix.h"

//Seeds the two given arrays with random values
void initMatrix(int* matA, int* matB, int size)
{
     srand(time(NULL));
           
     for (int row = 0; row < size; row++)
     {
          for (int col = 0; col < size; col++)
          {
               matA[row * size + col] = rand() % MAX_RAND;
               matB[row * size + col] = rand() % MAX_RAND;
          }
     }
}

//Prints the arrays.
void printMat(int* mat, int size)
{
     printf("Beginning of Matrix\n");

     for (int row = 0; row < size; row++)
     {
          for (int col = 0; col < size; col++)
          {
               printf(" %d ", mat[row * size + col]);
          }
          printf("\n");
     }
     printf("End of Matrix\n");
}

//Allocate those array. 
int* allocMat(int size)
{
     int* mat = (int*) malloc( sizeof(int) * size * size);

     return mat;
}

//Deallocate
void deleteMat(int* mat, int size)
{
     free(mat);
}

//Calculate, for testing purposes or sequential use
int* calMatrix(int* matA, int* matB, int size)
{
     //Allocate
     int* matC = allocMat(size);
   
     for (int row = 0; row < size; row++)
     {
          for (int col = 0; col < size; col++)
          {
               matC[row * size + col] = 0;
               for (int common = 0; common < size; common++)
               {
                    matC[row * size + col] = matC[row * size + col] 
                                   + matA[row * size + common] 
                                   * matB[common * size + col];   
               }
          }
     }
     return matC;
}

//Calculate the sub matrix given both array and where to start and stop
void subMatrixCal(int* matA, int* matB, int* matC, int rowWidth, int start, int stop)
{
     for (int row = start, idx = 0; row < stop; row++)
     {
          for (int col = 0; col < rowWidth; idx++, col++)
          {
               matC[idx] = 0;      
               for (int common = 0; common < rowWidth; common++)
               {
                    matC[idx] = matC[idx] 
                                + matA[row * rowWidth + common] 
                                * matB[common * rowWidth + col];
                      
               }
          }
     }
}

//Calculate the remaining rows
void remMatrixCal(int* matA, int* matB, int* matC, int rowWidth, int remainder)
{
     int start = rowWidth - remainder;
     int resultStart = rowWidth * (rowWidth - remainder);

     for (int row = start, idx = resultStart; row < rowWidth; row++)
     {
          for (int col = 0; col < rowWidth; idx++, col++)
          {
               matC[idx] = 0;      
               for (int common = 0; common < rowWidth; common++)
               {
                    matC[idx] = matC[idx] 
                                + matA[row * rowWidth + common] 
                                * matB[common * rowWidth + col];
                      
               }
          }
     }
}

