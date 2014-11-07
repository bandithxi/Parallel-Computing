#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
using namespace std;

// Global variables
const double G_CONSTANT = 6.67384 * pow(10, -11);
const double TIME_TICK = 0.1;

// Struct definitions
struct Vector
{
	double x;
	double y;
	double z;
};

struct Body
{
	int id;
	double mass;
	Vector pos;
	Vector vel;
	Vector force;
};

// Function headers
ifstream& operator>>( ifstream&, Body& );

ofstream& operator<<( ofstream&, Body& );

Vector& operator+=( Vector& main, const Vector& addition );

Vector calculateForce( Body&, Body& );

void updateVelocity( Body& );

void updatePosition( Body& );

long long getTimeInMillisec( timeval&, timeval& );

int main( int argc, char** argv )
{
	// Initialize variables
	int numBodies = atoi( argv[1] );
	int print = atoi( argv[3] );
	double maxTime = atoi( argv[2] );
	ifstream fin( "input" );
	ofstream fout;
	Body* bodies = NULL;
	struct timeval startTime;
	struct timeval endTime;

	// Open if enabled
	if( print == 1 )
	{
		fout.open( "output.csv" );
	}

	// Allocate and read the bodies
	bodies = new Body[ numBodies ];
	for( int i = 0; i < numBodies; i++ )
	{
		// Read
		fin >> bodies[ i ];
	}

	// Start timer
	gettimeofday( &startTime, NULL );

	// Time loop
	for( int t = 0.0; t < maxTime; t++ )
	{
		// For every body
		for( int i = 0; i < numBodies; i++ )
		{
			// Initialize variables
			bodies[i].force.x = bodies[i].force.y = bodies[i].force.z = 0;

			// Calculate force on both objects
			for( int j = 0; j < numBodies; j++ )
			{
				// Calculate force
				if( i != j )
					bodies[i].force += calculateForce( bodies[i], bodies[j] );
			}

			// If print enabled
			if( print == 1 ) { fout << bodies[i]; }
		}

		// For every body
		for( int i = 0; i < numBodies; i++ )
		{
			// Update
			updateVelocity( bodies[i] );
			updatePosition( bodies[i] );
		}
	}

	// End timer
	gettimeofday( &endTime, NULL );
	cout << "Duration (Milliseconds): " << 
		getTimeInMillisec( startTime, endTime ) << endl;
}

// Function implementations
Vector calculateForce( Body& one, Body& two )
{
	// Initialize variables
	double distance = sqrt( 
		pow((one.pos.x - two.pos.x), 2) + 
		pow((one.pos.y - two.pos.y), 2) + 
		pow((one.pos.z - two.pos.z), 2) );
	double distanceCubed = pow( distance, 3 );
	double numerator = (G_CONSTANT * one.mass * two.mass);
	Vector toReturn;
	
	// Edit objects
	toReturn.x = numerator * (two.pos.x - one.pos.x) / distanceCubed;
	toReturn.y = numerator * (two.pos.y - one.pos.y) / distanceCubed;
	toReturn.z = numerator * (two.pos.z - one.pos.z) / distanceCubed;

	// Return
	return toReturn;
}

void updateVelocity( Body& me )
{
	// Update variables
	me.vel.x += (me.force.x * TIME_TICK) / me.mass;
	me.vel.y += (me.force.y * TIME_TICK) / me.mass;
	me.vel.z += (me.force.z * TIME_TICK) / me.mass;
}

void updatePosition( Body& me )
{
	// Update variables
	me.pos.x += (me.vel.x * TIME_TICK);
	me.pos.y += (me.vel.y * TIME_TICK);
	me.pos.z += (me.vel.z * TIME_TICK);
}

ifstream& operator>>( ifstream& fin, Body& toEdit )
{
	// Read
	fin >> toEdit.id >> toEdit.mass >>
		toEdit.pos.x >> toEdit.pos.y >> toEdit.pos.z >>
		toEdit.vel.x >> toEdit.vel.y >> toEdit.vel.z;

	// Return
	return fin;
}

ofstream& operator<<( ofstream& fout, Body& me )
{
	// Initialize variables
	fout << me.id << ',' <<
		me.pos.x << ',' << me.pos.y << ',' << me.pos.z << ',' <<
		me.vel.x << ',' << me.vel.y << ',' << me.vel.z << ',' <<
		me.force.x << ',' << me.force.y << ',' << me.force.z << endl;

	// Return
	return fout;
}

Vector& operator+=( Vector& main, const Vector& addition )
{
	main.x += addition.x;
	main.y += addition.y;
	main.z += addition.z;
	return main;
}

long long getTimeInMillisec( timeval& start, timeval& end )
{
	// Initialize variables
	long long micro = (1000000 * (end.tv_sec - start.tv_sec)) +
		(end.tv_usec - start.tv_usec);
	return micro / 1000.0;
}
