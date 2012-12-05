#include <stdio.h>
#include <iostream>
#include "RandomWalker.h"

using namespace std;

int main(int argc, char* argv[])
{
	RandomWalker myWalker;

	myWalker.SetBoundaries( 200, 200, 500, 200 );

	// Since L = 1 in both x and y directions, x/L=0.4 and y/L=0.3 is calculated as follows
	int x_interest = static_cast<int>(myWalker.NODECOUNT * 0.4);
	int y_interest = static_cast<int>(myWalker.NODECOUNT * 0.3);

	vector<float> T;
	
	for (int i = 0; i < 50; i++)
	{
		clock_t wait = 1U * 1000 + clock();

		while ( wait > clock() ) continue;

		cout << "Current Percent Complete:  " << (static_cast<double>(i)/49)*100 << "%" << "		" << time(0) << endl;
		float T_Temp = myWalker.Solve_1Node(false, x_interest, y_interest);
		T.push_back(T_Temp);
	}

	ofstream myFile;
	myFile.open("500N25000W.csv");

	myFile << "Temperature" << endl;

	myFile.precision(6);

	for (unsigned int i = 0; i < T.size(); i++)
	{
		myFile  << T[i] << endl;
	}

	myFile.close();
	
	int CWAIT;
	cout << "Finished!" << endl;
	cin >> CWAIT;

	return 0;
}
