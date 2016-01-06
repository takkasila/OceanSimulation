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
	vector<vec3> originVertices;
	vector<complex<double>> eCompo;
	vector<complex<double>> h0;
	vector<complex<double>> h0Conj;
	vector<float> dispersion;
};
class Ocean : public Terrain
{
private:
	WavesVerticiesStaticVal vVar;

	complex<double> i = complex<double>(0,1);
	int N, M;
	float LengthX, LengthZ;
	float amplitude = 0.001f;
	vec2 windDir = normalize(vec2(1, 0.5f));
	float windSpeed = 32;
	float lambda = -1;

	default_random_engine generator;
	normal_distribution<double> gaussian;
	normal_distribution<double> gaussian2;

	complex<double> h_(int n, int m, double t);
	complex<double> h_0(int n, int m);
	double dispersion_relation(int n, int m);
	complex<double> philipsSpectrum(int n, int m);

public:
	Ocean(int nSampleX, int nSampleZ, float LengthX, float LengthY, int nInstanceX, int nInstanceZ);
	void UpdateWave(double time);
};
#endif