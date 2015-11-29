#include <stdio.h>
#include <string>
#include <vector>
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

#include "GLEW\glew.h"
#include "RenderObject.h"


void RenderObject::SetByArray(GLfloat data[], int length, vector<GLfloat> &targetData, GLuint &targetBuffer)
{
	targetData = vector<GLfloat>(data, data + length);
	SetByVector(targetData, targetData, targetBuffer);
}
void RenderObject::SetByVector(vector<GLfloat> data, vector<GLfloat> &targetData, GLuint &targetBuffer)
{
	targetData = data;
	glGenBuffers(1, &targetBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, targetBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * targetData.size(), &targetData[0], GL_STATIC_DRAW);
}
void RenderObject::SetByVectorVec3(vector<vec3> data, vector<GLfloat> &targetData, GLuint &targetBuffer)
{
	for (int i = 0; i < data.size(); i++)
	{
		targetData.push_back(data[i].x);
		targetData.push_back(data[i].y);
		targetData.push_back(data[i].z);
	}
	SetByVector(targetData, targetData, targetBuffer);
}
void RenderObject::SetByVectorVec2(vector<vec2> data, vector<GLfloat> &targetData, GLuint &targetBuffer)
{
	for (int i = 0; i < data.size(); i++)
	{
		targetData.push_back(data[i].x);
		targetData.push_back(data[i].y);
	}
	SetByVector(targetData, targetData, targetBuffer);
}


void RenderObject::SetVertex(GLfloat vertices [], int length)
{
	SetByArray(vertices, length, this->vertices, vertices_buffer);
}
void RenderObject::SetVertex(vector<GLfloat> vertices)
{
	SetByVector(vertices, this->vertices, vertices_buffer);
}
void RenderObject::SetVertex(vector<vec3> vertices)
{
	SetByVectorVec3(vertices, this->vertices, vertices_buffer);
}


void RenderObject::SetNormal(vector<GLfloat> normals)
{
	SetByVector(normals, this->normals, normals_buffer);
}
void RenderObject::SetNormal(GLfloat normals[], int length)
{
	SetByArray(normals, length, this->normals, normals_buffer);
}
void RenderObject::SetNormal(vector<vec3> normals)
{
	SetByVectorVec3(normals, this->normals, normals_buffer);
}


void RenderObject::SetColor(vector<GLfloat> colors)
{
	SetByVector(colors, this->colors, colors_buffer);
}
void RenderObject::SetColor(GLfloat colors [], int length)
{
	SetByArray(colors, length, this->colors, colors_buffer);
}
void RenderObject::SetColor(vector<vec3> colors)
{
	SetByVectorVec3(colors, this->colors, colors_buffer);
}


void RenderObject::SetUVs(vector<GLfloat> UVs)
{
	SetByVector(UVs, this->UVs, UVs_buffer);
}

void RenderObject::SetUVs(GLfloat UVs [], int length)
{
	SetByArray(UVs, length, this->UVs, UVs_buffer);
}

void RenderObject::SetUVs(vector<vec2> UVs)
{
	SetByVectorVec2(UVs, this->UVs, UVs_buffer);
}


void RenderObject::SetIndices(vector<unsigned int> indices)
{
	this->indices = indices;
	glGenBuffers(1, &indices_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &indices[0], GL_STATIC_DRAW);
}