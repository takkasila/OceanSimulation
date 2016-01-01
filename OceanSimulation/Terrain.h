#ifndef TERRAIN_H
#define TERRAIN_H

using namespace std;
using namespace glm;

class Terrain
{
protected:
	vector<vec3> vertices;
	vector<vec3> normals;

private:
	int x_width;
	int z_width;
	float x_spacing;
	float z_spacing;
	int x_instance;
	int z_instance;
	
	vector<unsigned int> indices;
	vector<vec2> UVs;
	vector<vec3> instance_offset;

public:
	Terrain(int x_width, int z_width, float length_x, float length_z, int x_instance, int z_instance);
	

	int GetWidth_X();
	int GetWidth_Z();
	float GetSpacing();
	int GetInstance_X();
	int GetInstance_Z();

	vector<vec3> GetVertices();
	vector<vec3> GetNormals();
	vector<unsigned int> GetIndices();
	vector<vec2> GetUVs();
	vector<vec3> GetInstance_offset();
};
#endif

