#include <stdio.h>
#include <vector>
#include <random>

#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Ocean.h"

Ocean::Ocean(int width, int length, GLfloat spacing, float Amplitude, vec2 WaveDir, float WaveLength, float GlobalSteepness, float WaveNumber) : Terrain(width, length, spacing)
{
	this->Amplitude = Amplitude;
	this->WaveDir = normalize(WaveDir);
	this->WaveLength = WaveLength;
	this->GlobalSteepness = GlobalSteepness;
	this->WaveNumber = WaveNumber;

	length_distribution = normal_distribution<float>(WaveLength, 3);
	amplitude_distribution = normal_distribution<float>(Amplitude / WaveNumber, .5);

	float radiance = atan2(this->WaveDir.y, this->WaveDir.x) * 180 / PI;
	direction_radiance_distribution = normal_distribution<float>(radiance, 13);
	
	for (int i = 0; i < WaveNumber; i++)
	{
		randomWave(waves[i]);
	}
}

void Ocean::randomWave(WaveParameter &wave)
{
	float direction_radiance_rand = direction_radiance_distribution(generator);
	float theta = direction_radiance_rand * PI / 180;
	wave.WaveDir.x = cos(theta);
	wave.WaveDir.y = sin(theta);

	wave.WaveLength = length_distribution(generator);
	wave.KAmpOverLen = amplitude_distribution(generator) / wave.WaveLength;
	wave.Speed = sqrt(G * 2 * PI / wave.WaveLength);

	int x = 5;
}