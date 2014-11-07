///////////////////////////////////////////////////////////////////////////////
// Header Files ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <thread>
#include <sys/time.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Struct Declarations ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct multData
   {
   int rowStart;
   int rowEnd;
   int* matrixA;
   int* matrixB;
   int* matrixC; 
   int squareDimension;
   };

///////////////////////////////////////////////////////////////////////////////
// Function Prototypes ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void threadMakeMatrices( multData argument );
void threadCalculation( multData argument );

///////////////////////////////////////////////////////////////////////////////
// Main Driver ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*NOTE: This program currently expects 2 inputs to be given. The program 
assumes that input 1 will be entered and correspond to the row and column
dimension of both matrix A and matrix B. (A and B will be square matrices of
the same size.) Input 2 will be the number of threadsto spawn.*/
int main( int argc, char* argv[] )
	{
	// Initialize variables
	int squareDimension = atoi( argv[1] ); // the m and n dimension for matrices 
														// a and b
	int* matrixA = new int[squareDimension * squareDimension]; // matrix being multiplied
	int* matrixB = new int[squareDimension * squareDimension]; // matrix being multiplied by
	int* matrixC = new int[squareDimension * squareDimension]; // resultant matrix
   int numThreads = atoi( argv[2] );
   thread threadList[numThreads];
   int rowsPerThread;
   int remainderRows;
   int startingRow = 0;
   multData threadArg;
	struct timeval start;
	struct timeval end;
	long long calcTime;

	// Create randomized seed
	srand( time(NULL) );

   // Calculate the number of rows to be handled by each thread
   rowsPerThread = squareDimension / numThreads;
   remainderRows = squareDimension % numThreads;

   // Initialize thread arguments
   threadArg.matrixA = matrixA;
   threadArg.matrixB = matrixB;
   threadArg.matrixC = matrixC;
   threadArg.squareDimension = squareDimension;

	// Spawn threads to generate matrices A and B
   for ( int threadNumber = 0; threadNumber < numThreads; threadNumber++)
      {
      // Set up argument for individual threads
      threadArg.rowStart = startingRow;
      threadArg.rowEnd = startingRow + rowsPerThread;

      // Create the thread
      threadList[threadNumber] = thread(threadMakeMatrices, threadArg);

		// Increment the starting row for next thread
		startingRow += rowsPerThread;
      }

	// Generate remainder rows for matrices A and B
	for ( int i = startingRow; i < squareDimension; i++ )
		{
		for ( int j = 0; j < squareDimension; j++ )
			{
			//For now stick with single digit positive ints
			matrixA[i * squareDimension + j] = rand() % 10;
			matrixB[i * squareDimension + j] = rand() % 10;
			}
		}

   // Loop to join the threads
   for ( int threadNumber = 0; threadNumber < numThreads; threadNumber++)
      {
      threadList[threadNumber].join();
      }


	// Reset the startingRow
	startingRow = 0;
   // Start timer, time only the calculations
   gettimeofday( &start, NULL );

   // Spawn threads for calculation
   for ( int threadNumber = 0; threadNumber < numThreads; threadNumber++)
      {
      // Set up argument for individual threads
      threadArg.rowStart = startingRow;
      threadArg.rowEnd = startingRow + rowsPerThread;

      // Create the thread
      threadList[threadNumber] = thread(threadCalculation, threadArg);

		// Increment the starting row for next thread
		startingRow += rowsPerThread;
      }

	// Do remainder rows
	for ( int i = startingRow; i < startingRow + remainderRows; i++ )
		{
		for ( int j = 0; j < squareDimension; j++ )
			{
			// Set the point in the resultant matrix to 0
			matrixC[i * squareDimension + j] = 0;

			// Do math
			for ( int k = 0; k < squareDimension; k++ )
				{
				matrixC[i * squareDimension + j] += matrixA[i * squareDimension + k] * matrixB[k * squareDimension + j];
				}
			}
		}	

   // Loop to join the threads
   for ( int threadNumber = 0; threadNumber < numThreads; threadNumber++)
      {
      threadList[threadNumber].join();
      }

	// Stop timer
	gettimeofday( &end, NULL );

	// Calculate time
	calcTime = ( ( (end.tv_sec * 1000000 ) + end.tv_usec ) - 
			 		( (start.tv_sec * 1000000 ) + start.tv_usec ) );

	// Output result
	cout << "Took " << calcTime << " microseconds with " << numThreads <<
			  " threads to compute a square matrix of dimension: " <<
			  squareDimension << endl;

	// Deallocate memory
	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;

	// Exit
	return 0;
	}

///////////////////////////////////////////////////////////////////////////////
// Function Implementations ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void threadCalculation( multData argument )
   {
   // Initialize variables
   int squareDimension = argument.squareDimension;

	// Perform matrix multiplication
	for ( int i = argument.rowStart; i < argument.rowEnd; i++ )
		{
		for ( int j = 0; j < squareDimension; j++ )
			{
			// Set the point in the resultant matrix to 0
			argument.matrixC[i * squareDimension + j] = 0;

			// Do math
			for ( int k = 0; k < squareDimension; k++ )
				{
				argument.matrixC[i * squareDimension + j] += argument.matrixA[i * squareDimension + k] * argument.matrixB[k * squareDimension + j];
				}
			}
		}
   }

void threadMakeMatrices( multData argument )
	{
   // Initialize variables
   int squareDimension = argument.squareDimension;

	// Generate rows for matrices A and B
	for ( int i = argument.rowStart; i < argument.rowEnd; i++ )
		{
		for ( int j = 0; j < squareDimension; j++ )
			{
			//For now stick with single digit positive ints
			argument.matrixA[i * squareDimension + j] = rand() % 10;
			argument.matrixB[i * squareDimension + j] = rand() % 10;
			}
		}
	}
