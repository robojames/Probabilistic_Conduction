#pragma once
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ppl.h>
#include <fstream>
#include <boost\random.hpp>
#include <boost\generator_iterator.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

class RandomWalker
{
public:
	// Default Constructor
	// int X_NODES -> Number of nodes in the X direction
	// int Y_NODES -> Number of nodes in the Y direction
	// int N_WALKERS -> Number of Random Walkers for each individual (x,y) pair 
	//
	// This function also properly sizes the vector<vector<float>> T, to avoid 
	// resizing issues (memory, time).  
	RandomWalker();

	// Public Methods

	// SetBoundaries - Sets boundary conditions for the 2D Monte Carlo Conduction Simulation,
	// currently only works with constant temperature boundary conditions.  Initializes T_EAST,
	// T_WEST, T_NORTH, and T_SOUTH.
	//
	// float T_EAST -> Temperature on the East face
	// float T_WEST -> Temperature on the West face
	// float T_NORTH -> Temperature on the North face
	// float T_SOUTH -> Temperature on the South face
	void SetBoundaries(float T_EAST, float T_WEST, float T_NORTH, float T_SOUTH);

	// Solve_AllNodes - Solves for the temperature field for all x and all y nodes
	//
	// bool flag -> if true, diagonals are included in the possible directions the random
	// walker can move, if false, then only the four cardinal directions are utilized.  This
	// flag is the same bool value used in int getDirection().
	vector<vector<float>> Solve_AllNodes(bool flag);

	// Solve_1Node - Solves for the temperature at one single node
	//
	// bool flag -> if true, diagonals are included in the possible directions the random walker
	// can move.  If false, then only the four cardinal directions will be utilized.  
	//
	// int x_node -> integer representing the node at which to solve for the temperature at in the x-direction
	// 
	// int y_node -> integer representing the node at which to solve for the temperature at in the y-direction
	float Solve_1Node(bool flag, int &x_node, int &y_node);

	float Solve_1Node(bool use_Diag, int &x_node, int &y_node, float q_triple, float grid_spacing, float thermal_Conductivity);


	float AnalyticalSolution();

	// WriteToFile - Writes the data to a .csv file for plotting (will eventually replace with OpenGL Plotter)
	//
	// char* FileName -> Indicates filename with which to name the output file
	void WriteToFile(char* FileName);

	int NODECOUNT;

private:
	float T_EAST, T_WEST, T_NORTH, T_SOUTH, L;

	vector<vector<float>> T;
	
	int N_WALKERS, X_NODES, Y_NODES, N_STEPS;
	
	// Move - gets random integer from getDirection() then, updates the nodal position based
	// on the movement of the random walker.  
	// X_POS_INI - integer value containing the initial nodal x-position of the random walker
	// Y_POS_INI - integer value containing the initial nodal y-position of the random walker
	void Move(int &X_POS_INI, int &Y_POS_INI, bool use_Diag);

	void Move(int &X_POS_INI, int &Y_POS_INI, bool use_Diag, int direction);

	// getDirection - Returns an random integer indicating a direction for the random walker
	// to travel.  The bool value use_Diag indicated whether the user wishes to include the
	// diagonal directions or not.
	int getDirection(bool use_Diag);

	// ApplyBoundaries() - Applies boundary conditions (currently only temperature) to each 
	// face of the 2D plate
	void Finalize();

	// Check_Boundary - Returns false if the random walker has not hit a boundary, and true
	// if the walker HAS reached a boundary.  
	// float &TEMP - Passes, by reference, a placeholder value for the current i,j temperature
	// X_POS - current nodal x position of the random walker
	// Y_POS - current nodal y position of the random walker
	bool Check_Boundary(float &TEMP, int X_POS, int Y_POS);
};

