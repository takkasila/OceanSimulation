#ifndef OCEAN_H
#define OCEAN_H
#include <random>
#include <complex>
#include <cmath>
#include "Terrain.h"

#define PI 3.1415926535897932384626433832795
#define G 9.80665

class Ocean : public Terrain
{
private:
	complex<double> i = sqrt(-1);
	int N, M;
	float LengthX, LengthZ;

	complex<double> h_(int n, int m, double t);
	

public:
	Ocean(int nSampleX, int nSampleZ, float LengthX, float LengthY, int nInstanceX, int nInstanceZ);
	void UpdateWave(double time);
};
#endif