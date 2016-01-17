#ifndef OCEAN_H
#define OCEAN_H
#include <random>
#include <complex>
#include <cmath>
#include "Terrain.h"

#define PI 3.1415926535897932384626433832795
#define G 9.80665

struct WavesVerticiesStaticVal
{
	vector<complex<float>> h0;
	vector<complex<float>> h0Conj;
	vector<float> dispersion;
};
class Ocean : public Terrain
{
private:
	complex<float> i = complex<float>(0,1);
	int N, M;
	float amplitude = 0.001f;
	vec2 windDir = normalize(vec2(1, 0.3f));
	float windSpeed = 30;
	float lambda = -1;

	default_random_engine generator;
	normal_distribution<float> gaussian;
	normal_distribution<float> gaussian2;

	complex<float> h_(int n, int m, float t);
	complex<float> h_0(int n, int m);
	float dispersion_relation(int n, int m);
	complex<float> philipsSpectrum(int n, int m);

public:
	WavesVerticiesStaticVal vVar;
	Ocean(int nSampleX, int nSampleZ, float LengthX, float LengthY, int nInstanceX, int nInstanceZ);
	float LengthX, LengthZ;

};
#endif