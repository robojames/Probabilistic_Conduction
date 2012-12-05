#include "RandomWalker.h"

using namespace std;
using namespace concurrency;
using namespace boost;

RandomWalker::RandomWalker()
{
	// Initializes both NODES and NWALKERS to 0
	int NODES(0), NWALKERS(0);

	// Has user input the number of nodes for both x and y directions
	cout << endl << "Enter Node count: ";
	cin >> NODES;

	// Has user input the number of random walkers required
	cout << endl << "Enter number of Random Walkers:  ";
	cin >> NWALKERS;

	// Sets initialized values equal to respective class members
	X_NODES = NODES; 
	Y_NODES = NODES;
	N_WALKERS = NWALKERS;
	NODECOUNT = NODES;

	// Resizes size(0) vector< vector<float> > to the number of nodes in the Y-direction.
	T.resize(Y_NODES);

	// Resizes STL vector again to make a vector of vectors, sizeof ( XNODES, YNODES )
	for (int i = 0; i < Y_NODES; i++)
			T[i].resize(X_NODES);	


}

float RandomWalker::AnalyticalSolution()
{
	// Initialize Analytic solution to 0
	float T_ANALYTIC(0.0f);
	
	// Initializes 4 different parts of the analytic equation
	float part_1(0.0f);
	float part_2(0.0f);
	float part_3(0.0f);
	float part_4(0.0f);

	// For loop which calculates the 'infinite' sum portion of the analytical expression for the temperature
	// field for a 2D plate.  
	//
	// 50 iterations is chosen to avoid overflow errors
	for (int n = 1; n <= 50; n++)
	{
		part_1 = (powf( -1.0f, static_cast<float>(n + 1.0f) ) + 1.0f) / static_cast<float>(n);
		part_2 = sinf( static_cast<float>(n * M_PI * 0.4f) );
		part_3 = sinhf( static_cast<float>(n * M_PI * 0.3f) );
		part_4 = sinhf( static_cast<float>(n * M_PI) );

		T_ANALYTIC += (part_1 * part_2 * (part_3 / part_4));
	}

	// Multiplies 'infinite' sum by 2/pi
	T_ANALYTIC *= static_cast<float>(2.0f / M_PI); 

	// Reverses non-dimensionalizing 
	T_ANALYTIC = T_ANALYTIC * (T_NORTH - T_EAST) + T_EAST;

	// Returns analytic temperature value
	return T_ANALYTIC;
}

void RandomWalker::SetBoundaries(float T_E, float T_W, float T_N, float T_S)
{
	// Sets the boundary temperature values to the passed in value
	T_EAST = T_E;
	T_WEST = T_W;
	T_NORTH = T_N;
	T_SOUTH = T_S;
}

void RandomWalker::WriteToFile(char* FileName)
{
	// If T vector is empty, exits function (if function does not correctly exit then a
	// runtime error occurs).
	if (T.empty())
	{
		cout << "T Vector is Empty" << endl;
		return;
	}

	// Opens filestream object myFile
	ofstream myFile;

	// Creates a new file based on passed in string (filename)
	myFile.open(FileName);

	// Iterates over all x and y nodes so values are properly displayed in Excel when viewed as 
	// a .csv file
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			myFile << T[i][j] << ",";
		}

		myFile << endl;
	}

	// Closes filestream to avoid potential errors
	myFile.close();
}

float RandomWalker::Solve_1Node(bool use_Diag, int &x_node, int &y_node)
{
	// Initializes typedef to avoid typing mt19937 all over the place
	typedef mt19937 RNGType;

	time_t myTime = time(0);

	// Sets up RNGType's (mt19937) seed value based on the time
	RNGType range(myTime);
	
	// Defines PDF (uniform) and range (integer from 1 - 4)
	uniform_int<> random_int(1,4);
	
	// Initializes the random number variate generator function for use as dice()
	variate_generator< RNGType, uniform_int<> > dice(range, random_int);

	// Initialize Temperature 'Temp' to 0
	float Temp(0.0f);

	// Initializes iterations to 0
	int iterations_Done(0);

	// Initializes percent complete value
	float percent_Complete(0.0f);

	// Main for loop--iterates over each walker.  Note, this was initially set up as
	// a parallel_for, however the new random number generator (which is far more accurately random...)
	// is not 'thread-safe', and so the parallel-for with lambda expression was reduced to a simple
	// for loop.
	for (int k = 0; k < N_WALKERS; k++)
	{
		// Initializes walker position to passed in value
		int X_POS = x_node;
		int Y_POS = y_node;

		// Initializes bool value indicating if the walker has reached a boundary yet
		bool reached_boundary(false);

		// While loop which performs the random walk
		while (!reached_boundary)
		{
			Move( X_POS, Y_POS, use_Diag, dice());

			reached_boundary = Check_Boundary( Temp, X_POS, Y_POS );
		}

		// Increments number of iterations (walkers) to correctly display % completion to the user
		iterations_Done++;
		
		// Displays percent completion every 50 walkers to avoid computational delays due to displaying
		// percent completion
		if ( !(k % 50) ) {

			percent_Complete = iterations_Done/static_cast<float>(N_WALKERS) * 100;

			//cout << "Percent Complete:  " << percent_Complete << "%" << endl;
		}
	}	

	// Returns temperature value
	return (Temp/static_cast<float>(N_WALKERS));
}

float RandomWalker::Solve_1Node(bool use_Diag, int &x_node, int &y_node, float q_triple, float grid_spacing, float thermal_Conductivity)
{
	typedef mt19937 RNGType;

	RNGType range( static_cast<uint32_t>(time(0)) );
	
	uniform_int<> random_int(1,4);
	
	variate_generator< RNGType, uniform_int<> > dice(range, random_int);

	float Temp(0.0f);

	float SUM_Steps(0.0f);

	int iterations_Done(0);
	float percent_Complete(0);

	for (int k = 0; k < N_WALKERS; k++) {

		int X_POS = x_node;
		int Y_POS = y_node;

		N_STEPS = 0;

		bool reached_boundary(false);

		while (!reached_boundary)
		{
			Move( X_POS, Y_POS, use_Diag, dice());

			N_STEPS++;
			
			reached_boundary = Check_Boundary( Temp, X_POS, Y_POS );
		}

		SUM_Steps += N_STEPS;

		iterations_Done++;

		if ( !(k % 50) ) {
		percent_Complete = iterations_Done/static_cast<float>(N_WALKERS) * 100;

		cout << "Percent Complete:  " << percent_Complete << "%" << endl;
		}
	}

	// Returns temperature, taking into consideration the effect of the source terms and number of steps
	Temp = Temp/static_cast<float>(N_WALKERS) + (q_triple * grid_spacing * grid_spacing)/(4.0f * thermal_Conductivity) * (1.0f / N_WALKERS) * SUM_Steps;

	return Temp;
}

vector<vector<float>> RandomWalker::Solve_AllNodes(bool use_Diag)
{
	typedef mt19937 RNGType;

	RNGType range( static_cast<uint32_t>(time(0)) );
	
	uniform_int<> random_int(1,4);
	
	variate_generator< RNGType, uniform_int<> > dice(range, random_int);

	parallel_for (int(0), X_NODES, [&](int i)
	{
		cout << "Working on Row: " << i << " of " << X_NODES << endl;

		for (int j = 1; j < Y_NODES; j++)
		{
			for (int k = 0; k < N_WALKERS; k++)
			{
				int X_POS = i;
				int Y_POS = j;

				N_STEPS = 0;

				bool reached_Boundary(false);

				while (!reached_Boundary)
				{
					Move(X_POS, Y_POS, use_Diag, dice());
					
					reached_Boundary =	Check_Boundary(T[i][j], X_POS, Y_POS);
				}
			}
		}
	}, static_partitioner());
	
	Finalize();

	return T;
}

int RandomWalker::getDirection(bool use_Diag)
{

	if (!use_Diag)
	{
		int random_integer = (rand() % 4) + 1;
		return random_integer;

	}
	else
	{
		int random_integer = (rand() % 8) + 1;
		return random_integer;

	}

}

void RandomWalker::Move(int &X_POS_INI, int &Y_POS_INI, bool use_Diag, int direction)
{
	N_STEPS++;

	if (!use_Diag) {

	switch (direction)
		{
			case 1: // Moves North
				Y_POS_INI++;
				break;
			case 2: // Moves South
				Y_POS_INI--;
				break;
			case 3: // Moves West
				X_POS_INI--;
				break;
			case 4: // Moves East
				X_POS_INI++;
				break;
		}
	}
	else
	{
		switch (direction)
			{
				case 1: // Moves North
					Y_POS_INI++;
					break;
				case 2: // Moves South
					Y_POS_INI--;
					break;
				case 3: // Moves West
					X_POS_INI--;
					break;
				case 4: // Moves East
					X_POS_INI++;
					break;
				case 5: // Moves NorthEast		--> BEGIN DIAGONAL DIRECTIONS <--
					Y_POS_INI++;
					X_POS_INI++;
					break;
				case 6: // Moves NorthWest
					X_POS_INI--;
					Y_POS_INI++;
					break;
				case 7: // Moves SouthWest
					Y_POS_INI--;
					X_POS_INI--;
					break;
				case 8: // Moves SouthEast
					Y_POS_INI--;
					X_POS_INI++;
					break;
			}
	}
}

bool RandomWalker::Check_Boundary(float &TEMP, int X_POS, int Y_POS)
{
	if (X_POS >= X_NODES)
	{
		TEMP += T_EAST;
		return true;
	}
	if (X_POS <= 0)
	{
		TEMP += T_WEST;
		return true;
	}

	if (Y_POS >= Y_NODES)
	{
		TEMP += T_NORTH;
		return true;
	}

	if (Y_POS <= 0)
	{
		TEMP += T_SOUTH;
		return true;
	}

	return false;
}

void RandomWalker::Finalize()
{
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
		
			T[i][j] = T[i][j]/static_cast<float>(N_WALKERS);

			if (j==0)
			{
				T[i][j] = T_SOUTH;
			}

			if (j==(Y_NODES-1))
			{
				T[i][j] = T_NORTH;
			}

			if (i==0)
			{
				T[i][j] = T_WEST;
			}

			if (i==(X_NODES-1))
			{
				T[i][j] = T_EAST;
			}

		}
	}
}