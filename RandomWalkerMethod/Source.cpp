#include <stdio.h>
#include <iostream>
#include "RandomWalker.h"
#include "ViewFactorAnalysis.h"

using namespace std;

int main(int argc, char* argv[])
{
	//RandomWalker myWalker;

	//myWalker.SetBoundaries( 200, 200, 500, 200 );

	//// Since L = 1 in both x and y directions, x/L=0.4 and y/L=0.3 is calculated as follows
	//int x_interest = static_cast<int>(myWalker.NODECOUNT * 0.4);
	//int y_interest = static_cast<int>(myWalker.NODECOUNT * 0.3);
	
	ViewFactorAnalysis myAnalysis(50, 0.5, 0.5);
	
	myAnalysis.Solve();
	
	
	int CWAIT;
	cout << "Finished!" << endl;
	cin >> CWAIT;

	return 0;
}
