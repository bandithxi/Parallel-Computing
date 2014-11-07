#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Constant
const char* SPACE = "    ";

// Main
int main( int argc, char** argv )
{
	// Initialize variables
	int numBodies = atoi( argv[1] );
	ofstream fout( "input", iostream::out );

	// Loop
	for( int i = 0; i < numBodies; i++ )
	{
		// ID
		fout <<

			// ID
			i << SPACE <<

			// Mass
			rand() % 999 + 1 << SPACE <<

			// Position
			(float) rand() / (float) (RAND_MAX / 100.0) << SPACE <<
			(float) rand() / (float) (RAND_MAX / 100.0) << SPACE <<
			(float) rand() / (float) (RAND_MAX / 100.0) << SPACE <<

			// Velocity
			(float) rand() / (float) (RAND_MAX / 10.0) << SPACE << 
			(float) rand() / (float) (RAND_MAX / 10.0) << SPACE <<
			(float) rand() / (float) (RAND_MAX / 10.0) << endl;
	}

	// Return success
	return 0;
}
