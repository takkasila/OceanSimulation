#include <stdio.h>
#include <vector>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "LoadShader.h"
#include "Controls.h"

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

int main()
{
#pragma region Init
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
#pragma endregion

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("scene_v_shader.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("scene_geometry_shader.glsl", GL_GEOMETRY_SHADER);
	shaderProgram.AddShader("scene_f_shader.glsl", GL_FRAGMENT_SHADER);
	GLuint programID = shaderProgram.LinkProgram();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Testing PLANE
	GLfloat plane_v_data[] = {
		-1, 0, -1
		, -1, 0, 1
		, 1, 0, 1
		, -1, 0, -1
		, 1, 0, 1
		, 1, 0, -1
	};
	GLuint plane_v_buffer;
	glGenBuffers(1, &plane_v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, plane_v_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_v_data), plane_v_data, GL_STATIC_DRAW);

	GLfloat plane_colors [] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // Bottom-left
	};
	GLuint plane_color_buffer;
	glGenBuffers(1, &plane_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, plane_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_colors), plane_colors, GL_STATIC_DRAW);

	GLuint MVPID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "View");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "Model");
	GLuint renderModeID = glGetUniformLocation(programID, "renderMode");

	GLuint mvp_uniform_block;
	glGenBuffers(1, &mvp_uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, mvp_uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mvp_uniform_block);
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1);
		model = glm::scale(model, vec3(5, 5, 5));
		mat4 MVP = projection  * view * model;
		
		glUseProgram(programID);

		//glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(MVP), &MVP[0][0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, plane_v_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, plane_color_buffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glDrawArrays(GL_POINTS, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	
	glfwTerminate();

	return 0;
	
}