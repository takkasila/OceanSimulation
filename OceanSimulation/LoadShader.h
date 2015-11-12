#ifndef LOADSHADER_H
#define LOADSHADER_H

class ShaderGenerator
{
private:
	GLuint programID;
	std::vector<GLuint> shaderObjects;
	GLint Result = GL_FALSE;
	int InfoLogLength;

public:
	ShaderGenerator()
	{
		programID = glCreateProgram();
	}

	void AddShader(const char* shader_file_path, GLenum shader_type);
	GLuint LinkProgram();

};


#endif