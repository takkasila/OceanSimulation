#ifndef TERRAIN_H
#define TERRAIN_H

using namespace std;
using namespace glm;

class Terrain
{
private:
	int x_width;
	int z_width;
	float spacing;
	int x_instance;
	int z_instance;

	vector<vec3> vertices;
	vector<unsigned int> indices;
	vector<vec2> UVs;
	vector<vec3> instance_offset;

public:
	Terrain(int width, int length, GLfloat spacing, int x_instance, int z_instance);
	int GetWidth_X();
	int GetWidth_Z();
	float GetSpacing();
	int GetInstance_X();
	int GetInstance_Z();

	vector<vec3> GetVertices();
	vector<unsigned int> GetIndices();
	vector<vec2> GetUVs();
	vector<vec3> GetInstance_offset();
};
#endif

