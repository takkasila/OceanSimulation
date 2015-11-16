#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "GLEW\glew.h"
#include "RenderObject.h"


void RenderObject::SetVertex(vector<GLfloat> vertices_in)
{
	vertices = vertices_in;
	glGenBuffers(1, &vertices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}
void RenderObject::SetVertex(GLfloat vertices_in [], int length)
{
	vertices = vector<GLfloat>(vertices_in, vertices_in + length);
	SetVertex(vertices);
}


void RenderObject::SetNormal(vector<GLfloat> normals_in)
{
	normals = normals_in;
	glGenBuffers(1, &normals_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);
}
void RenderObject::SetNormal(GLfloat normals_in[], int length)
{
	normals = vector<GLfloat>(normals_in, normals_in + length);
	SetNormal(normals);
}

void RenderObject::SetColor(vector<GLfloat> colors_in)
{
	colors = colors_in;
	glGenBuffers(1, &colors_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colors_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);
}
void RenderObject::SetColor(GLfloat colors_in [], int length)
{
	colors = vector<GLfloat>(colors_in, colors_in + length);
	SetColor(colors);
}