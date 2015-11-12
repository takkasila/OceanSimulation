#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "GLEW\glew.h"

#include "LoadShader.h"

void LoadShader(GLuint* programID, const char* shader_file_path, GLenum shader_type, GLuint* shader_object)
{
	GLuint ShaderID = glCreateShader(shader_type);

	// Read file
	string ShaderCode;
	std::ifstream ShaderStream(shader_file_path, ios::in);
	if (ShaderStream.is_open())
	{
		std::string Line = "";
		
		while (getline(ShaderStream, Line))
		{
			ShaderCode += "\n" + Line;
		}
		ShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", shader_file_path);
	char const * SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer, NULL);
	glCompileShader(ShaderID);

	// Check Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Attach Shader
	fprintf(stdout, "Attach shader : %s\n", shader_file_path);
	glAttachShader(*programID, ShaderID);

	
}