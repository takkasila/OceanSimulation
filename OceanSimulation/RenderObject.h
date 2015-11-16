#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

using namespace std;
using namespace glm;
class RenderObject
{
private:
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> colors;

	void SetByArray(GLfloat data [], int length, vector<GLfloat> &targetData, GLuint &targetBuffer);
	void SetByVector(vector<GLfloat> data, vector<GLfloat> &targetData, GLuint &targetBuffer);
	void SetByVectorVec3(vector<vec3> data, vector<GLfloat> &targetData, GLuint &targetBuffer);

public:
	GLuint vertices_buffer;
	GLuint normals_buffer;
	GLuint colors_buffer;
	
	void SetVertex(GLfloat vertices[], int length);
	void SetVertex(vector<GLfloat> vertices);
	void SetVertex(vector<vec3> vertices);

	void SetNormal(GLfloat normals[], int length);
	void SetNormal(vector<GLfloat> normals);
	void SetNormal(vector<vec3> normals);

	void SetColor(GLfloat colors [], int length);
	void SetColor(vector<GLfloat> colors);
	void SetColor(vector<vec3> colors);
};

#endif