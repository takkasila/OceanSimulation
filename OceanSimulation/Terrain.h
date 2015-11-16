#ifndef TERRAIN_H
#define TERRAIN_H

using namespace std;
using namespace glm;
class Terrain
{
private:
	int x_width;
	int z_width;
	vector<vec3> vertices;
	vector<unsigned int> indices;

public:
	Terrain(int width, int length, int spacing);
	int GetWidth_X();
	int GetWidth_Z();
	vector<vec3> GetVertices();
	vector<unsigned int> GetIndices();
	int spacing;
};
#endif

