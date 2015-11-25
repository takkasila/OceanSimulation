#include <stdio.h>
#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Ocean.h"

Ocean::Ocean(int width, int length, GLfloat spacing) : Terrain(width, length, spacing)
{
	float PI = 3.1415926535897932384626433832795;

	WaveNumber = 1;
	GlobalSteepness = 0.6;
	
	//float GlobalSteepness = 0.6;	// ragne from 0 to 1
	float WaveLength = 8;
	float Speed = sqrt(9.81 * 2 * PI / WaveLength);
	float KAmpOverLen = 0.05;
	vec2 WaveDir = normalize(vec2(1, 0.5));

	//waves[0].GlobalSteepness = GlobalSteepness;
	waves[0].KAmpOverLen = KAmpOverLen;
	waves[0].Speed = Speed;
	waves[0].WaveDir = WaveDir;
	waves[0].WaveLength = WaveLength;
}