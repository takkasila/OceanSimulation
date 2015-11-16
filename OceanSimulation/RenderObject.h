#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

class RenderObject
{
private:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;

public:
	GLuint vertices_buffer;
	GLuint normals_buffer;
	GLuint colors_buffer;
	
	void SetVertex(std::vector<GLfloat> vertices_in);
	void SetVertex(GLfloat vertices_in[], int length);

	void SetNormal(std::vector<GLfloat> normals_in);
	void SetNormal(GLfloat normals_in[], int length);

	void SetColor(std::vector<GLfloat> colors_in);
	void SetColor(GLfloat colors_in[], int length);
};

#endif