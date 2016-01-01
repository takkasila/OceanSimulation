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
}

void Ocean::UpdateWave(double time)
{
	for (int pos = 0; pos < vertices.size(); pos++)
	{
		complex<double> sum_comp(0, 0);
		for (int n = 0; n < N; n++) for (int m = 0; m < M; m++)
		{
			vec2 K;
			K.x = (2 * PI*n - PI*N) / LengthX;
			K.y = (2 * PI*m - PI*M) / LengthZ;
			complex<double> e_component =
				exp(i*(double) (vertices[pos].x*K.x)
				+ i*(double) (vertices[pos].z*K.y));
			sum_comp += h_(n, m, time) * e_component;
		}
		vertices[pos].y = sum_comp.real();
		printf("%d\n", pos);
	}
	
}

complex<double> Ocean::h_(int n, int m, double t)
{
	return exp(i);
}