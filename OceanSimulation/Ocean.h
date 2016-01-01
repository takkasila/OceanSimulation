#ifndef OCEAN_H
#define OCEAN_H
#include <random>
#include "Terrain.h"

#define PI 3.1415926535897932384626433832795
#define G 9.80665

class Ocean : public Terrain
{
private:
	

public:

	Ocean(int width, int length, GLfloat spacing, int x_instance, int z_instance, float Amplitude, vec2 WaveDir, float WaveLength, float GlobalSteepness, float WaveNumber);
};
#endif