#ifndef VIEWFACTORANALYSIS_H
#define VIEWFACTORANALYSIS_H


#include <boost\random.hpp>
#include <boost\generator_iterator.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

// ViewFactorAnalysis.h
// Solves for the View factor of a plate arranged as shown below:
//   |
// h |
// A2|
//   |_____________________
//		A1   	w
// Where the view factor found is F(1->2)

class ViewFactorAnalysis
{
public:
	// Default Constructor
	// Initializes default member variables
	//
	// N_PHOTONS -> Number of photons to 'launch' off of surface
	// w -> width of area 1
	// h -> height of area 2
	ViewFactorAnalysis(int N_PHOTONS, float w, float h);

	// Member Functions
	//
	// Solve - Returns float value of the view factor found
	float Solve();

	// AnalyticSolution - Returns float value of analytic view factor found
	float AnalyticSolution();
private:
	// Float values describing both the width and height of the plates analyzed
	float width, height;

	// Number of photons that are launched and hit
	int N, N_hits;

	// Used to store value of minimum and maximum of Beta
	float Beta_Min, Beta_Max;

	bool LaunchPhoton(float launch_angle);

	void Calculate_Beta(float &B_MIN, float &B_MAX, float x_pos);
};


#endif
