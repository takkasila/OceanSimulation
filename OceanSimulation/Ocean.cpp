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
	
}