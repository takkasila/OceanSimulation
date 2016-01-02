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
	complex<double> i = complex<double>(0,1);
	int N, M;
	float LengthX, LengthZ;
	float amplitude = 1;
	vec2 windDir = normalize(vec2(1, 1));
	float windSpeed = 2;

	default_random_engine generator;
	normal_distribution<double> gaussian;

	complex<double> h_(int n, int m, double t);
	complex<double> h_0(int n, int m);
	double dispersion_relation(int n, int m);
	complex<double> philipsSpectrum(int n, int m);
	

public:
	Ocean(int nSampleX, int nSampleZ, float LengthX, float LengthY, int nInstanceX, int nInstanceZ);
	void UpdateWave(double time);
};
#endif