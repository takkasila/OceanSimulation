#include <stdio.h>
#include <vector>
#include <random>
#include <complex>
#include <cmath>

#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Ocean.h"
Ocean::Ocean(int nSampleX, int nSampleZ, float LengthX, float LengthZ, int nInstanceX, int nInstanceZ)
	: Terrain(nSampleX, nSampleZ, LengthX, LengthZ, nInstanceX, nInstanceZ)
{
	this->N = nSampleX;
	this->M = nSampleZ;
	this->LengthX = LengthX;
	this->LengthZ = LengthZ;

	int i = 0;
	for (int n = 0; n < N; n++) for (int m = 0; m < M; m++)
	{
		vertices[i].x -= N / 2.0;
		vertices[i].z -= M / 2.0;

		originVertices.push_back(vertices[i++]);
	}

	gaussian = normal_distribution<double>(0, 1);
	UpdateWave(0);
}

void Ocean::UpdateWave(double time)
{
	for (int pos = 0; pos < vertices.size(); pos++)
	{
		complex<double> sum_comp(0, 0);
		vec2 sum_displacement(0, 0);
		vec3 sum_normal(0, 0, 0);
		for (int n = 0; n < N; n++) for (int m = 0; m < M; m++)
		{
			vec2 K;
			K.x = (2 * PI*n - PI*N) / LengthX;
			K.y = (2 * PI*m - PI*M) / LengthZ;

			complex<double> e_component =
				exp(i*(double) (vertices[pos].x*K.x)
				+ i*(double) (vertices[pos].z*K.y));

			complex<double> h_compo = h_(n, m, time);
			complex<double> hc = h_compo * e_component;
			sum_comp += hc;

			float lengthK = length(K);
			if (lengthK == 0)
			{
				continue;
			}
			sum_displacement += vec2(K.x / length(K) *   hc.imag(), K.y / length(K) * hc.imag());

			sum_normal += vec3( - K.x * hc.imag(), 0, -K.y * hc.imag());
		}
		vertices[pos].y = sum_comp.real();
		vertices[pos].x = originVertices[pos].x + lambda * sum_displacement.x;
		vertices[pos].z = originVertices[pos].z + lambda * sum_displacement.y;
		normals[pos] = vec3(0, 1, 0) - sum_normal;

	}
	
}

complex<double> Ocean::h_(int n, int m, double t)
{
	complex<double> h0, h0Conj, e_component, e_componentConj;
	
	h0 = h_0(n, m);
	h0Conj = conj(h_0(-n, -m));

	double dispersion = dispersion_relation(n, m);
	
	e_component = exp(i *(double) (dispersion * t));
	e_componentConj = exp(-i*(double) (dispersion * t));

	int lmao11 = 0;
	int lmao12 = lmao11 + 1;
	return h0 * e_component + h0Conj * e_componentConj;
}

double Ocean::dispersion_relation(int n, int m)
{
	return sqrt( G * sqrt( n*n + m*m) );
}

complex<double> Ocean::h_0(int n, int m)
{
	complex<double> complexRand(gaussian(generator), gaussian(generator));
	return complexRand * sqrt( philipsSpectrum(n, m) / 2.0);
}

complex<double> Ocean::philipsSpectrum(int n, int m)
{
	vec2 K;
	K.x = (2 * PI*n - PI*N) / LengthX;
	K.y = (2 * PI*m - PI*M) / LengthZ;
	
	double KLength = length(K);
	// Critical if n = N/2
	if (KLength == 0) return 0;

	double waveLength = windSpeed*windSpeed / G;
	complex<double> e_component = exp(-1.0 / pow((KLength * waveLength), 2));

	double wave_component = pow(abs(dot(normalize(K), windDir)), 6) / pow(KLength, 4);
	return amplitude * wave_component * e_component;
}