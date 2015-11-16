#include <stdio.h>
#include <string>
#include <vector>
#include "GLEW\glew.h"
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "Terrain.h"


Terrain::Terrain(int width, int length, int spacing)
{

	this->width = width;
	this->length = length;
	this->spacing = spacing;

	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < length; z++)
		{
			vertices.push_back(vec3(x * spacing, 0, z * spacing));
		}
	}
}

int Terrain::GetWidth()
{
	return width;
}

int Terrain::GetLength()
{
	return length;
}

vector<vec3> Terrain::GetVertices()
{
	return vertices;
}