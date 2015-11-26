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
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;
vec3 BGColor(0.5, 0.5, 0.5);

int InitProgram();
void SendUniformMVP();
void SendUniformWaveParameters(Ocean oceanObj);

int main()
{
	if (InitProgram() != 0)
		return -1;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("v_ocean.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("f_input_color.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	ShaderGenerator normalVisualShaderProgram;
	normalVisualShaderProgram.AddShader("v_ocean.glsl", GL_VERTEX_SHADER);
	normalVisualShaderProgram.AddShader("g_normals_visualize.glsl", GL_GEOMETRY_SHADER);
	normalVisualShaderProgram.AddShader("f_input_color.glsl", GL_FRAGMENT_SHADER);
	GLuint normalVisualShaderProgramID = normalVisualShaderProgram.LinkProgram();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Ocean oceanObj(128, 128, .25);
	RenderObject oceanObjBuffer;
	oceanObjBuffer.SetVertex(oceanObj.GetVertices());
	oceanObjBuffer.SetIndices(oceanObj.GetIndices());


	GLuint mvp_uniform_block;
	glGenBuffers(1, &mvp_uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 3, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mvp_uniform_block);

	GLuint wave_uniform_block;
	glGenBuffers(1, &wave_uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, wave_uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) + sizeof(float) * 4 + sizeof(vec2), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, wave_uniform_block);

	GLuint timeID;
	GLuint waveNumberID;
	GLuint globalSteepnessID;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSetTime(0);
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);

		timeID = glGetUniformLocation(shaderProgramID, "time");
		waveNumberID = glGetUniformLocation(shaderProgramID, "WaveNumber");
		globalSteepnessID = glGetUniformLocation(shaderProgramID, "GlobalSteepness");
		glUniform1f(timeID, glfwGetTime());
		glUniform1i(waveNumberID, oceanObj.WaveNumber);
		glUniform1f(globalSteepnessID, oceanObj.GlobalSteepness);

		glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
		SendUniformMVP();
		glBindBuffer(GL_UNIFORM_BUFFER, wave_uniform_block);
		SendUniformWaveParameters(oceanObj);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, oceanObjBuffer.vertices_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanObjBuffer.indices_buffer);
		glDrawElements(GL_TRIANGLES, oceanObj.GetIndices().size(), GL_UNSIGNED_INT, (void*) 0);

		// Normal
		glUseProgram(normalVisualShaderProgramID);

		timeID = glGetUniformLocation(normalVisualShaderProgramID, "time");
		waveNumberID = glGetUniformLocation(normalVisualShaderProgramID, "WaveNumber");
		globalSteepnessID = glGetUniformLocation(normalVisualShaderProgramID, "GlobalSteepness");
		glUniform1f(timeID, glfwGetTime());
		glUniform1i(waveNumberID, oceanObj.WaveNumber);
		glUniform1f(globalSteepnessID, oceanObj.GlobalSteepness);

		glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
		SendUniformMVP();
		glBindBuffer(GL_UNIFORM_BUFFER, wave_uniform_block);
		SendUniformWaveParameters(oceanObj);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, oceanObjBuffer.vertices_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanObjBuffer.indices_buffer);
		glDrawElements(GL_TRIANGLES, oceanObj.GetIndices().size(), GL_UNSIGNED_INT, (void*) 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);


	glDeleteProgram(shaderProgramID);
	glDeleteProgram(normalVisualShaderProgramID);
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

	//glClearColor(0.8, 0.8, 0.8, 0);
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

void SendUniformWaveParameters(Ocean oceanObj)
{
	glBufferData(GL_UNIFORM_BUFFER, sizeof(WaveParameter) * MAX_WAVE_NUMBER, &oceanObj.waves[0], GL_STATIC_DRAW);
}