#include <stdio.h>

#include "GLEW\glew.h";
#include "GLFW\glfw3.h";
#include "glm\glm.hpp";

#include "LoadShader.h";

using namespace glm;
using namespace std;

typedef unsigned long DWORD;
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;

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

	window = glfwCreateWindow(window_width, window_height, "OpenGLSkeleton", NULL, NULL);

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
	glClearColor(230 / 255.0
		, 74 / 255.0
		, 25 / 255.0
		, 0);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
#pragma endregion

	GLuint programID = LoadShaders("scene_v_shader.glsl", "scene_f_shader.glsl");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint MVPID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "View");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "Model");
	GLuint renderModeID = glGetUniformLocation(programID, "renderMode");

	do
	{

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	
	glfwTerminate();

	return 0;
	
}