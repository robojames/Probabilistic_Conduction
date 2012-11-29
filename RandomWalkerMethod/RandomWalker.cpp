#include "RandomWalker.h"

using namespace std;
using namespace concurrency;

RandomWalker::RandomWalker(int XNODES, int YNODES, int NWALKERS)
{
	// Initializes values of class properties
	X_NODES = XNODES; 
	Y_NODES = YNODES;
	N_WALKERS = NWALKERS;
	
	// Resizes size(0) vector< vector<float> > to the number of nodes in the Y-direction.
	T.resize(Y_NODES);

	// Resizes STL vector again to make a vector of vectors, sizeof ( XNODES, YNODES )
	for (int i = 0; i < Y_NODES; i++)
			T[i].resize(X_NODES);		
	

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
	T.clear();

	srand(static_cast<unsigned>(time(0)));

	float Temp;

	parallel_for (int(0), N_WALKERS, [&](int k)
	{
		int X_POS = x_node;
		int Y_POS = y_node;

		N_STEPS = 0;

		bool reached_boundary(false);

		while (!reached_boundary)
		{
			Move( X_POS, Y_POS, use_Diag);

			reached_boundary = Check_Boundary( Temp, X_POS, Y_POS );
		}
	});

	cout << "T [ " << x_node << " , " << y_node << " ] = " << Temp;

	return Temp;
}

vector<vector<float>> RandomWalker::Solve_AllNodes(bool use_Diag)
{
	srand(static_cast<unsigned>(time(0)));

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
					Move(X_POS, Y_POS, use_Diag);
					
					reached_Boundary =	Check_Boundary(T[i][j], X_POS, Y_POS);
				}

				//cout << "Steps:  " << N_STEPS << endl;
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

void RandomWalker::Move(int &X_POS_INI, int &Y_POS_INI, bool use_Diag)
{
	int dir = getDirection(use_Diag);

	N_STEPS++;

	if (!use_Diag) {

	switch (dir)
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
		switch (dir)
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