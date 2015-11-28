#ifndef OCEAN_H
#define OCEAN_H
#include <random>
#include "Terrain.h"

#define MAX_WAVE_NUMBER 50
#define PI 3.1415926535897932384626433832795
#define G 9.80665
struct WaveParameter
{
	vec2 WaveDir;
	float WaveLength;
	float Speed;
	
	float KAmpOverLen;
	float padding;
	float padding2;
	float padding3;
};
class Ocean : public Terrain
{
private:
	default_random_engine generator;
	normal_distribution<float> length_distribution;
	normal_distribution<float> amplitude_distribution;
	normal_distribution<float> direction_radiance_distribution;
	
	void randomWave(WaveParameter &wave);

public:
	float Amplitude;
	vec2 WaveDir;
	float WaveLength;
	float GlobalSteepness;
	float WaveNumber;

	WaveParameter waves[MAX_WAVE_NUMBER];
	Ocean(int width, int length, GLfloat spacing, float Amplitude, vec2 WaveDir, float WaveLength, float GlobalSteepness, float WaveNumber);
};
#endif