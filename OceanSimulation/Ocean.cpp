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

	gaussian = normal_distribution<double>(0, 1);
	gaussian2 = normal_distribution<double>(0, 1);
	int pos = 0;
	for (int n = 0; n < N; n++) for (int m = 0; m < M; m++)
	{
		vertices[pos].x -= N / 2.0;
		vertices[pos].z -= M / 2.0;

		vVar.originVertices.push_back(vertices[pos]);

		vec2 K;
		K.x = (2 * PI*n - PI*N) / LengthX;
		K.y = (2 * PI*m - PI*M) / LengthZ;

		vVar.h0.push_back(h_0(n, m));
		vVar.h0Conj.push_back(conj(h_0(-n, -m)));
		vVar.dispersion.push_back(dispersion_relation(n, m));

		pos++;
	}

	//UpdateWave(0);
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

			complex<double> h_compo = h_(n, m, time);
			complex<double> e_compo = exp(i*(double) (vertices[pos].x*K.x)
				+ i*(double) (vertices[pos].z*K.y));
			complex<double> hc = h_compo * e_compo;
			
			sum_comp += hc;
			
			sum_normal += vec3( - K.x * hc.imag(), 0, -K.y * hc.imag());
			
			float lengthK = length(K);
			if (lengthK == 0)
				continue;
			sum_displacement += vec2(K.x / length(K) *   hc.imag(), K.y / length(K) * hc.imag());

		}
		vertices[pos].y = sum_comp.real();
		vertices[pos].x = vVar.originVertices[pos].x + lambda * sum_displacement.x;
		vertices[pos].z = vVar.originVertices[pos].z + lambda * sum_displacement.y;
		normals[pos] = normalize(vec3(0, 1, 0) - sum_normal);

	}
}


complex<double> Ocean::h_(int n, int m, double t)
{
	int pos = n * N + m;
	complex<double> e_component, e_componentConj;
	
	e_component = exp(i *(double) (vVar.dispersion[pos] * t));
	e_componentConj = exp(-i*(double) (vVar.dispersion[pos] * t));

	return vVar.h0[pos] * e_component + vVar.h0Conj[pos] * e_componentConj;
}

double Ocean::dispersion_relation(int n, int m)
{
	float w0 = 2 * PI / 200.0;
	vec2 K;
	K.x = (2 * PI*n - PI*N) / LengthX;
	K.y = (2 * PI*m - PI*M) / LengthZ;
	double dispersionTerm = sqrt(G * sqrt(K.x * K.x + K.y * K.y));
	return floor(dispersionTerm/w0) * w0;
}

complex<double> Ocean::h_0(int n, int m)
{
	complex<double> complexRand(gaussian(generator), gaussian2(generator));
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

	double wave_component = pow(dot(normalize(K), windDir), 4) / pow(KLength, 4);
	
	float damping = 0.001;
	complex<double> damp_compo = exp(-KLength * KLength * waveLength * waveLength * damping * damping);
	
	return amplitude * wave_component * e_component * damp_compo;
}