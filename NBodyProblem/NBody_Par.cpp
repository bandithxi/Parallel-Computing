#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <mpi.h>
using namespace std;

// Global constants
const double G_CONSTANT = 6.67384 * pow(10, -11);
const double TIME_TICK = 3600.0;

// Global variables
int ID;
int SIZE;
double* mass;
double* pos;
double* vel;
double* force;

// Function headers
void copy( double*, double* );
void calculateForce( double* result, int one, int two );
void updateVelocity( double*, double*, double );
void updatePosition( double*, double* );
long long getTimeInMillisec( timeval&, timeval& );

// Main Function
int main( int argc, char** argv )
{
	// Initialize MPI
	MPI_Init( &argc, &argv );

	// Get ID and size
	MPI_Comm_size( MPI_COMM_WORLD, &SIZE );
	MPI_Comm_rank( MPI_COMM_WORLD, &ID );

	// Initialize variables
	int n = atoi( argv[1] );
	int print = atoi( argv[3] );
	double maxTime = atof( argv[2] );
	double buffer[ 3 ];
	struct timeval startTime;
	struct timeval endTime;

	// Set processor specific values
	int temp = n / SIZE;
	int bodiesPerProc = ( temp * SIZE != n ) ? temp + 1 : temp;
	int bStart = ID * bodiesPerProc;
	int bEnd = (ID + 1) * bodiesPerProc;

	// Allocate arrays
	mass = new double[ bodiesPerProc * SIZE ];
	pos = new double[ bodiesPerProc * SIZE * 3 ];
	vel = new double[ bodiesPerProc * SIZE * 3 ];
	force = new double[ bodiesPerProc * SIZE * 3 ];

	///////////////////////////////////////////////////////////////////////
	// Setup
	///////////////////////////////////////////////////////////////////////
	if( ID == 0 )
	{
		// Initialize variables
		ifstream fin( "input", std::iostream::in );

		// For Loop
		for( int i = 0; i < n; i++ )
		{
			// Read id and mass
			fin >> mass[ i ];
			MPI_Bcast( &mass[ i ], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );

			// Read body position and velocity
			fin >> pos[ (i * 3) + 0 ] 
					>> pos[ (i * 3) + 1 ] 
					>> pos[ (i * 3) + 2 ];
			MPI_Bcast( &pos[ i * 3 ], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD );
			fin >> pos[ (i * 3) + 0 ] 
					>> pos[ (i * 3) + 1 ] 
					>> vel[ (i * 3) + 2 ];
			MPI_Bcast( &vel[ i * 3 ], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD );
		}
	}
	else
	{
		// Loop and recieve data
		for( int i = 0; i < n; i++ )
		{
			// Recieve mass, position, and velocity
			MPI_Bcast( buffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
			mass[ i ] = buffer[ 0 ];
			MPI_Bcast( buffer, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD );
			copy( &pos[ i * 3 ], buffer );
			MPI_Bcast( buffer, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD );
			copy( &vel[ i * 3 ], buffer );
		}
	}

	///////////////////////////////////////////////////////////////////////
	// Work
	///////////////////////////////////////////////////////////////////////
	// Start timer
	if( ID == 0 ) { gettimeofday( &startTime, NULL ); }

	// Initialize output file
	ofstream fout;
	if( print == 1 && ID == 0 )
	{ 
		// Open and write header
		fout.open( "output.csv" );
		fout << n << ' ' << maxTime << endl;
	}

	// Loop for entirity of ticks
	for( int i = 0; i < maxTime; i++ )
	{
		// Loop through every body
		for( int j = bStart; j < bEnd; j++ )
		{
			// Zero out the force
			force[ (j * 3) + 0 ] = 
				force[ (j * 3) + 1 ] = force[ (j * 3) + 2 ] = 0;

			// Calculate forces on object
			for( int k = 0; k < n; k++ )
			{
				// Calculate force
				if( j != k ) { calculateForce( force, j, k ); }
			}
		}

		// Loop through every body
		for( int j = bStart; j < bEnd; j++ )
		{
			// Update position and then velocity
			updatePosition( &pos[ j * 3 ], &vel[ j * 3 ] );
			updateVelocity( &vel[ j * 3 ], &force[ j * 3 ], mass[ j ] );
		}

		// Sync up with new values 
		MPI_Allgather( &pos[ ID * bodiesPerProc * 3 ], bodiesPerProc * 3,
			MPI_DOUBLE, pos, bodiesPerProc * 3, MPI_DOUBLE, MPI_COMM_WORLD );
		MPI_Allgather( &vel[ ID * bodiesPerProc * 3 ], bodiesPerProc * 3,
			MPI_DOUBLE, vel, bodiesPerProc * 3, MPI_DOUBLE, MPI_COMM_WORLD );

		// Output if enabled
		if( print == 1 && ID == 0 )
		{
			// Loop and output
			for( int i = 0; i < n; i++ )
			{
				fout << pos[ (i * 3) + 0 ] << ' ' <<
					pos[ (i * 3) + 1 ] << ' ' <<
					pos[ (i * 3) + 2 ] << endl;
			}
		}
	}

	// End timer
	if( ID == 0 )
	{
		gettimeofday( &endTime, NULL );
		cout << "Duration (Milliseconds): " << 
			getTimeInMillisec( startTime, endTime ) << endl;
	}

	// Finalize
	MPI_Finalize();

	// Deallocate
	delete[] mass;
	delete[] pos;
	delete[] vel;
	delete[] force;
	mass = pos = vel = force = NULL;

	// Return
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Function implementations
void copy( double* dest, double* source )
{
	// Copy first three
	dest[ 0 ] = source[ 0 ];
	dest[ 1 ] = source[ 1 ];
	dest[ 2 ] = source[ 2 ];
}

void calculateForce( double* result, int one, int two )
{
	// Initialize variables
	double pos_x_diff = pos[ (one * 3) + 0 ] - pos[ (two * 3) + 0 ];
	double pos_y_diff = pos[ (one * 3) + 1 ] - pos[ (two * 3) + 1 ];
	double pos_z_diff = pos[ (one * 3) + 2 ] - pos[ (two * 3) + 2 ];
	double distance = sqrt( 
		pow( pos_x_diff, 2) +
		pow( pos_y_diff, 2) +
		pow( pos_z_diff, 2) );
	double distanceCubed = pow( distance, 3 );
	double numerator = (G_CONSTANT * mass[ one ] * mass[ two ]);
	
	// Edit objects
	result[ (one * 3) + 0 ] += numerator * (-1.0 * pos_x_diff) / distanceCubed;
	result[ (one * 3) + 1 ] += numerator * (-1.0 * pos_y_diff) / distanceCubed;
	result[ (one * 3) + 2 ] += numerator * (-1.0 * pos_z_diff) / distanceCubed;
}

void updateVelocity( double* vel, double* force, double bodyMass )
{
	// Update variables
	vel[ 0 ] += (force[ 0 ] * TIME_TICK) / bodyMass;
	vel[ 1 ] += (force[ 1 ] * TIME_TICK) / bodyMass;
	vel[ 2 ] += (force[ 2 ] * TIME_TICK) / bodyMass;
}

void updatePosition( double* pos, double* vel )
{
	// Update variables
	pos[ 0 ] += vel[ 0 ] * TIME_TICK;
	pos[ 1 ] += vel[ 1 ] * TIME_TICK;
	pos[ 2 ] += vel[ 2 ] * TIME_TICK;
}

long long getTimeInMillisec( timeval& start, timeval& end )
{
	// Initialize variables
	long long micro = (1000000 * (end.tv_sec - start.tv_sec)) +
		(end.tv_usec - start.tv_usec);
	return micro / 1000.0;
}
