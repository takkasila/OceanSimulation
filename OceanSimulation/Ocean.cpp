#include <stdio.h>
#include <vector>
#include <random>

#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Ocean.h"

Ocean::Ocean(int width, int length, GLfloat spacing, int x_instance, int z_instance, float Amplitude, vec2 WaveDir, float WaveLength, float GlobalSteepness, float WaveNumber) 
	: Terrain(width, length, spacing, x_instance, z_instance)
{
	this->Amplitude = Amplitude;
	this->WaveDir = normalize(WaveDir);
	this->WaveLength = WaveLength;
	this->GlobalSteepness = GlobalSteepness;
	this->WaveNumber = WaveNumber;

	length_distribution = normal_distribution<float>(WaveLength, 2);
	amplitude_distribution = normal_distribution<float>(Amplitude / WaveNumber, .3);

	float radiance = atan2(this->WaveDir.y, this->WaveDir.x) * 180 / PI;
	direction_radiance_distribution = normal_distribution<float>(radiance, 20);

	float mainSpeed = sqrt(2 * G * PI / WaveLength);
	float mainOmega = 2 * PI / WaveLength;
	mainPhase = mainSpeed * mainOmega;
	phase_distribution = normal_distribution<float>(mainPhase, 1 );
	
	for (int i = 0; i < WaveNumber; i++)
	{
		randomWave(waves[i]);
	}
}

void Ocean::randomWave(WaveParameter &wave)
{
	//WaveLength <= OceanSize (in world)
	// WaveLength = 1/(n * TileSize)

	wave.WaveLength = length_distribution(generator);

	wave.WaveLength = (GetWidth_X() - 1) * GetSpacing();
	wave.WaveLength /= 2;


	wave.KAmpOverLen = amplitude_distribution(generator) / wave.WaveLength;
	wave.Speed = sqrt(G * 2 * PI / wave.WaveLength);
	wave.Phase = phase_distribution(generator);

	float direction_radiance_rand = direction_radiance_distribution(generator);
	float theta = direction_radiance_rand * PI / 180;
	wave.WaveDir.x = cos(theta);
	wave.WaveDir.y = sin(theta);

	//wave.WaveDir = vec2(1, 0);
	//wave.WaveDir = normalize(wave.WaveDir);
}