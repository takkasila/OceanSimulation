#ifndef TERRAIN_H
#define TERRAIN_H

using namespace std;
using namespace glm;
class Terrain
{
private:
	int width;
	int length;
	vector<vec3> vertices;

public:
	Terrain(int width, int length, int spacing);
	int GetWidth();
	int GetLength();
	vector<vec3> GetVertices();
	int spacing;
};
#endif

