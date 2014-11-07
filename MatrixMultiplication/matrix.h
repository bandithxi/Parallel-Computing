#ifndef MATRIX_H
#define MATRIX_H

//Handles only square matrices
#include <time.h> //Time function
#include <stdlib.h> //Rand function
#include <stdio.h> //I/O functions

//Fill matrices with random numbers 
void initMatrix(int* matA, int* matB, int size);

//Print the matrix 
void printMat(int* mat, int size);

//Allocate matrix
int* allocMat(int size);

//Sequential matrix multiplication
int* calMatrix(int* matA, int* matB, int size);

//Deallocate matrix
void deleteMat(int* mat, int size);

//Matrix multiplication for part of the matrix
void subMatrixCal(int* matA, int* matB, int* matC, int rowWidth, int start, int stop);

//Matrix multiplication for remainder of the matrix
void remMatrixCal(int* matA, int* matB, int* matC, int rowWidth, int remainder);

//The maximum a random number can be is 100 for simplicity.
#define MAX_RAND 10

#endif
