#include <stdio.h>
#include <iostream>
#include "RandomWalker.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Added comments to test GitHub BS
	RandomWalker myWalker( 200, 200 , 2500 );

	myWalker.SetBoundaries( 500, 250, 200, 150 );

	myWalker.Solve_AllNodes(false);

	myWalker.WriteToFile("TestFile2parrelel.csv");

	cout << "Finished!" << endl;


	return 0;
}
