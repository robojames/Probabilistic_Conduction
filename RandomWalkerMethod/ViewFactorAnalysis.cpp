#include "ViewFactorAnalysis.h"

using namespace std;
using namespace boost;

ViewFactorAnalysis::ViewFactorAnalysis(int N_PHOTONS, float w, float h)
{
	// Initialize Member variables
	width = w;
	height = h;
	N = N_PHOTONS;

	// Initialize Beta's
	Beta_Max = 0.0f;
	Beta_Min = 0.0f;
}

float ViewFactorAnalysis::AnalyticSolution()
{
	// Initializes view factor to 0
	float F12 = 0;

	float H = height / width;

	// Calculates the analytic value based on
	// F(1->2) = 0.5*(1 + H - sqrt(1 + H^2) )
	F12 = 0.5f * (1.0f + H - sqrtf(1.0f + powf(H, 2.0f) ) );

	return F12;
}

float ViewFactorAnalysis::Solve()
{
	float F12 = 0;

	// Build random number function
	// Need float from 0 to 1
	typedef mt19937 RNGType;

	time_t myTime = time(0);

	RNGType range(myTime);

	uniform_real<> random_real(0, 1);

	variate_generator< RNGType, uniform_real<> > dice(range, random_real);

	for (int i = 0; i < N; i++)
	{
		// Set x position along base of Area 1
		float x_pos = width * static_cast<float>(dice());
		
		// Calculate Beta Min, and Beta Max
		Calculate_Beta(Beta_Min, Beta_Max, x_pos);

		// "Launch" photon and see if it falls within the range of beta_min to beta_max
		bool hit = LaunchPhoton( static_cast<float>(dice()) );

		if (hit)
		{
			N_hits++;
		}

	}

	F12 = static_cast<float>(N_hits)/static_cast<float>(N);

	return F12;

}

bool ViewFactorAnalysis::LaunchPhoton(float rand_launchangle)
{
	bool within_Range = false;

	float beta_Launch = 0.0f;

	beta_Launch = asinf( (2.0f * rand_launchangle) - 1 );

	//cout << "Launch angle (degrees):  " << beta_Launch*(180.0f/M_PI) << "       ";
	
	if (beta_Launch <= Beta_Max && beta_Launch >= Beta_Min)
		within_Range = true;
	else
		within_Range = false;

	//cout << within_Range << endl;

	return within_Range;
}

void ViewFactorAnalysis::Calculate_Beta(float &B_MIN, float &B_MAX, float x_pos)
{
	// Starting with beta = 0 being a photon that flies off to the East and
	// basic geometry...

	// The maximum beta for the prescribed geometry will always be pi/2 radians
	B_MAX = (static_cast<float>(M_PI) / 2.0f);

	B_MIN = (static_cast<float>(M_PI) / 2.0f) - atan2f(height , x_pos);
}

