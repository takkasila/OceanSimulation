#ifndef OCEAN_H
#define OCEAN_H
#include "Terrain.h"

#define WAVE_NUMBER 1
struct WaveParameter
{
	vec2 WaveDir;
	float GlobalSteepness;
	float WaveLength;
	
	float Speed;
	float KAmpOverLen;
	float padding;
	float padding2;
};
class Ocean : public Terrain
{
public:
	int waveNumber;
	WaveParameter waves[WAVE_NUMBER];
	Ocean(int width, int length, GLfloat spacing);
};
#endif