#include <stdio.h>
#include <vector>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "LoadShader.h"
#include "Controls.h"
#include "RenderObject.h"
#include "Terrain.h"

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
void SendUniformWaveParameters();


struct WavePar
{
	vec2 WaveDir;
	int WaveNumber;
	float GlobalSteepness;
	float WaveLength;
	float Speed;
	float KAmpOverLen;
	float padding;
};
int main()
{
	if (InitProgram() != 0)
		return -1;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("v_ocean.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("f_input_color.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Terrain oceanObj(256, 256, 0.1);
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

	GLuint timeID = glGetUniformLocation(shaderProgramID, "time");
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSetTime(0);
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);

		glUniform1f(timeID, glfwGetTime());

		glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
		SendUniformMVP();
		glBindBuffer(GL_UNIFORM_BUFFER, wave_uniform_block);
		SendUniformWaveParameters();
		

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

void SendUniformWaveParameters()
{
	int WaveNumber = 1;
	float GlobalSteepness = 0.6;	// ragne from 0 to 1
	float WaveLength = 8;
	float Speed = sqrt(9.81 * 2 * pi<double>() / WaveLength);

	float KAmpOverLen =0.05;
	vec2 WaveDir = normalize(vec2(1, 0.5));


	WavePar waves[2];
	waves[1].GlobalSteepness = GlobalSteepness;
	waves[1].KAmpOverLen = KAmpOverLen;
	waves[1].Speed = Speed;
	waves[1].WaveDir = WaveDir;
	waves[1].WaveLength = WaveLength;
	waves[1].WaveNumber = WaveNumber;

	glBufferData(GL_UNIFORM_BUFFER, sizeof(WavePar) * 2, &waves[0], GL_STATIC_DRAW);

	/*glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec2), &WaveDir);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec2), sizeof(int), &WaveNumber);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec2)+ sizeof(int), sizeof(float), &GlobalSteepness);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec2)+ sizeof(int) + sizeof(float), sizeof(float), &WaveLength);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec2)+ sizeof(int) + sizeof(float) * 2, sizeof(float), &Speed);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec2)+ sizeof(int) + sizeof(float) * 3, sizeof(float), &KAmpOverLen);
*/
}