#ifndef OCEAN_H
#define OCEAN_H
#include "Terrain.h"

#define MAX_WAVE_NUMBER 50
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
public:
	float WaveNumber;
	float GlobalSteepness;
	WaveParameter waves[MAX_WAVE_NUMBER];
	Ocean(int width, int length, GLfloat spacing);
};
#endif