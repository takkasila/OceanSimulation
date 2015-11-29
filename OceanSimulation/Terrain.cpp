#include <stdio.h>
#include <vector>
#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Terrain.h"

#define INDEX(x, z, x_width) (x+z*x_width)
Terrain::Terrain(int x_width, int z_width, GLfloat spacing, int x_instance, int z_instance)
{

	this->x_width = x_width;
	this->z_width = z_width;
	this->spacing = spacing;
	this->x_instance = x_instance;
	this->z_instance = z_instance;

	for (int z = 0; z < z_width; z++)
	{
		for (int x = 0; x < x_width; x++)
		{
			vertices.push_back(vec3(x * spacing, 0, z * spacing));
			
			UVs.push_back(vec2(z/ (float)z_width, x/ (float)x_width));
			// Filling indices
			if (z > 0 && x > 0)
			{
				// Bottom triangle
				indices.push_back(INDEX((x - 1), (z - 1), x_width));
				indices.push_back(INDEX((x - 1), (z - 0), x_width));
				indices.push_back(INDEX((x - 0), (z - 0), x_width));

				// Top triangle
				indices.push_back(INDEX((x - 1), (z - 1), x_width));
				indices.push_back(INDEX((x - 0), (z - 0), x_width));
				indices.push_back(INDEX((x - 0), (z - 1), x_width));
			}
		}
	}

	float x_space = (x_width - 1) * spacing;
	float z_space = (z_width - 1) * spacing;

	for (int z = 0; z < z_instance; z++)
	{
		int a;
		for (int x = 0; x < x_instance; x++)
		{
			instance_offset.push_back(vec3(x * x_space,0, z * z_space));
			int b;
		}
	}
}

int Terrain::GetWidth_X()
{
	return x_width;
}

int Terrain::GetWidth_Z()
{
	return z_width;
}

float Terrain::GetSpacing()
{
	return spacing;
}

int Terrain::GetInstance_X()
{
	return x_instance;
}

int Terrain::GetInstance_Z()
{
	return z_instance;
}

vector<vec3> Terrain::GetVertices()
{
	return vertices;
}

vector<unsigned int> Terrain::GetIndices()
{
	return indices;
}

vector<vec2> Terrain::GetUVs()
{
	return UVs;
}

vector<vec3> Terrain::GetInstance_offset()
{
	return instance_offset;
}