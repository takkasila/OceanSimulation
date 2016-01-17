#include <stdio.h>
#include <vector>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "LoadShader.h"
#include "Controls.h"
#include "RenderObject.h"
#include "Ocean.h"

using namespace glm;
using namespace std;


typedef unsigned long DWORD;
extern "C" {	// Force using Nvidia GPU. Turn 0 if don't want to.
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000000;
}

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;
vec3 BGColor(226 / 255.0, 243 / 255.0, 241 / 255.0);

int InitProgram();
void SendUniformMVP();
void SendUniformWaveParameters(Ocean oceanObj);

int main()
{
	if (InitProgram() != 0)
		return -1;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("v_ocean_dft.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("f_ambient_diffuse_specular.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Ocean oceanObj(128, 128, 60, 60, 3, 3);
	RenderObject oceanObjBuffer;
	oceanObjBuffer.SetVertex(oceanObj.GetVertices());
	oceanObjBuffer.SetNormal(oceanObj.GetNormals());
	oceanObjBuffer.SetIndices(oceanObj.GetIndices());

	GLuint mvp_uniform_block;
	glGenBuffers(1, &mvp_uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 3, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mvp_uniform_block);

	GLuint LightPosition_worldspaceID = glGetUniformLocation(shaderProgramID, "LightPosition_worldspace");
	GLuint EyePositionID = glGetUniformLocation(shaderProgramID, "EyePosition");
	GLuint DirectionalLight_direction_worldspaceID = glGetUniformLocation(shaderProgramID, "DirectionalLight_direction_worldspace");
	GLuint instance_offsetID = glGetUniformLocation(shaderProgramID, "instance_offset");
	
	vector<vec3> instance_offset_vec3 = oceanObj.GetInstance_offset();

#pragma region WaveParameter to Shader

	GLuint timeID = glGetUniformLocation(shaderProgramID, "time");
	GLuint sampleNID = glGetUniformLocation(shaderProgramID, "N");
	GLuint sampleMID = glGetUniformLocation(shaderProgramID, "M");
	GLuint LengthXID = glGetUniformLocation(shaderProgramID, "LengthX");
	GLuint LengthZID = glGetUniformLocation(shaderProgramID, "LengthZ");

	// h0
	GLuint h0_uniform = glGetUniformLocation(shaderProgramID, "h0");
	GLuint h0_data_buffer;
	glGenBuffers(1, &h0_data_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, h0_data_buffer);
	glBufferData(GL_TEXTURE_BUFFER, oceanObj.vVar.h0.size() * sizeof(float), &oceanObj.vVar.h0[0], GL_STATIC_DRAW);

	GLuint h0_texID;
	glGenTextures(1, &h0_texID);
	glBindTexture(GL_TEXTURE_BUFFER, h0_texID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, h0_data_buffer);
	
	// h0Conj
	GLuint h0Conj_uniform = glGetUniformLocation(shaderProgramID, "h0Conj");
	GLuint h0Conj_data_buffer;
	glGenBuffers(1, &h0Conj_data_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, h0Conj_data_buffer);
	glBufferData(GL_TEXTURE_BUFFER, oceanObj.vVar.h0Conj.size() * sizeof(float), &oceanObj.vVar.h0Conj[0], GL_STATIC_DRAW);

	GLuint h0Conj_texID;
	glGenTextures(1, &h0Conj_texID);
	glBindTexture(GL_TEXTURE_BUFFER, h0Conj_texID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, h0Conj_data_buffer);

	// dispersion
	GLuint dispersion_uniform = glGetUniformLocation(shaderProgramID, "dispersion");
	GLuint dispersion_data_buffer;
	glGenBuffers(1, &dispersion_data_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, dispersion_data_buffer);
	glBufferData(GL_TEXTURE_BUFFER, oceanObj.vVar.dispersion.size() * sizeof(float), &oceanObj.vVar.dispersion[0], GL_STATIC_DRAW);

	GLuint dispersion_texID;
	glGenTextures(1, &dispersion_texID);
	glBindTexture(GL_TEXTURE_BUFFER, dispersion_texID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, dispersion_data_buffer);

#pragma endregion

	glfwSetTime(0);
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);
		
		glUniform1f(timeID, (float)glfwGetTime());
		glUniform1i(sampleNID, oceanObj.GetWidth_X());
		glUniform1i(sampleMID, oceanObj.GetWidth_Z());
		glUniform1f(LengthXID, oceanObj.LengthX);
		glUniform1f(LengthZID, oceanObj.LengthZ);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, h0_texID);
		glUniform1i(h0_uniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_BUFFER, h0Conj_texID);
		glUniform1i(h0Conj_uniform, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_BUFFER, dispersion_texID);
		glUniform1i(dispersion_uniform, 2);

		glUniform3f(LightPosition_worldspaceID, 128 * .25 / 2, 7, 128 * .25 / 2);
		vec3 eyePos = getEyePos();
		glUniform3f(EyePositionID, eyePos.x, eyePos.y, eyePos.z);
		glUniform3f(DirectionalLight_direction_worldspaceID, -1, -1, -1);
		glUniform3fv(instance_offsetID, instance_offset_vec3.size(), &instance_offset_vec3[0].x);

		glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
		SendUniformMVP();
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, oceanObjBuffer.vertices_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanObjBuffer.indices_buffer);
		glDrawElementsInstanced(GL_TRIANGLES, oceanObj.GetIndices().size(), GL_UNSIGNED_INT, (void*) 0, instance_offset_vec3.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	glDeleteProgram(shaderProgramID);
	glfwTerminate();
	return 0;
}

int InitProgram()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init glfw\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	window = glfwCreateWindow(window_width, window_height, "OceanSimulation", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open glfw window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to init GLEW");
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, window_width / 2, window_height / 2);

	glClearColor(BGColor.x, BGColor.y, BGColor.z, 1);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	return 0;
}


void SendUniformMVP()
{
	computeMatricesFromInputs();
	mat4 projection = getProjectionMatrix();
	mat4 view = getViewMatrix();
	mat4 model = mat4(1);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &model[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &view[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, sizeof(mat4), &projection[0][0]);
}