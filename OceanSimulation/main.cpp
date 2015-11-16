#include <stdio.h>
#include <vector>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "LoadShader.h"
#include "Controls.h"
#include "RenderObject.h"

using namespace glm;
using namespace std;


typedef unsigned long DWORD;
extern "C" {	// Force using Nvidia GPU. Turn 0 if don't want to.
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;

GLuint shaderGuy();

int InitProgram();
void SendUniformMVP();

int main()
{
	int programStatus = InitProgram();
	if (programStatus != 0)
		return programStatus;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("v_simple.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("f_input_color.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	ShaderGenerator normalVisualizeShaderProgram;
	normalVisualizeShaderProgram.AddShader("v_normals_visualize.glsl", GL_VERTEX_SHADER);
	normalVisualizeShaderProgram.AddShader("g_normals_visualize.glsl", GL_GEOMETRY_SHADER);
	normalVisualizeShaderProgram.AddShader("f_input_color.glsl", GL_FRAGMENT_SHADER);
	GLuint normalVisualizeShaderProgramID = normalVisualizeShaderProgram.LinkProgram();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Testing PLANE
	RenderObject plane;
	GLfloat plane_v_data[] = {
		-1, 0, -1,
		-1, 0, 1,
		1, 0, 1,
		-1, 0, -1,
		1, 0, 1,
		1, 0, -1
	};

	GLfloat plane_colors [] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	GLuint isUseColorID = glGetUniformLocation(shaderProgramID, "isUseColor");

	GLfloat plane_normal [] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	plane.SetVertex(plane_v_data, sizeof(plane_v_data)/sizeof(GLfloat));
	plane.SetColor(plane_colors, sizeof(plane_colors)/sizeof(GLfloat));
	plane.SetNormal(plane_normal, sizeof(plane_normal)/sizeof(GLfloat));

	GLuint mvp_uniform_block;
	glGenBuffers(1, &mvp_uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 3, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mvp_uniform_block);
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SendUniformMVP();
		
		// Render scene
		glUseProgram(shaderProgramID);

		glUniform1i(isUseColorID, GL_TRUE);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, plane.vertices_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, plane.colors_buffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render normal visualize
		glUseProgram(normalVisualizeShaderProgramID);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, plane.vertices_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, plane.normals_buffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLOAT, 0, (void*) 0);

		glDrawArrays(GL_TRIANGLES, 0, 12);

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
	glClearColor(0, 0, 0, 1);

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