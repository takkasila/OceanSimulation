#include <stdio.h>
#include <vector>
#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Terrain.h"

#define INDEX(x, z, x_width) (x+z*x_width)
Terrain::Terrain(int x_width, int z_width, GLfloat spacing)
{

	this->x_width = x_width;
	this->z_width = z_width;
	this->spacing = spacing;

	for (int z = 0; z < z_width; z++)
	{
		for (int x = 0; x < x_width; x++)
		{
			vertices.push_back(vec3(x * spacing, 0, z * spacing));

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
}

int Terrain::GetWidth_X()
{
	return x_width;
}

int Terrain::GetWidth_Z()
{
	return z_width;
}

vector<vec3> Terrain::GetVertices()
{
	return vertices;
}

vector<unsigned int> Terrain::GetIndices()
{
	return indices;
}