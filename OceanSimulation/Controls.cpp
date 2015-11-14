#include "GLFW\glfw3.h"
extern GLFWwindow* window;
extern float window_width;
extern float window_height;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

using namespace glm;

#include "controls.h"

mat4 ViewMatrix;
mat4 ProjectionMatrix;
mat4 getViewMatrix()
{
	return ViewMatrix;
}
mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

// Initial 
vec3 position = vec3(0, 2, 5);

// turn toward -Z
//float horizontalAngle = (5 / 4.0f)*pi<float>();
float horizontalAngle = pi<float>();
float verticalAngle = -pi<float>() / 5;
float initialFoV = 45;

float speed = 5;
float mouseSpeed = 0.005f;

void computeMatricesFromInputs()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//reset mouse position for nextframe
	glfwSetCursorPos(window, window_width / 2, window_height / 2);

	horizontalAngle += mouseSpeed*float(window_width / 2 - xpos);
	verticalAngle += mouseSpeed*float(window_height / 2 - ypos);

	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle)
		, sin(verticalAngle)
		, cos(verticalAngle) * cos(horizontalAngle)
		);
	vec3 right = vec3(
		sin(horizontalAngle - pi<float>() / 2)
		, 0
		, cos(horizontalAngle - pi<float>() / 2)
		);

	vec3 up = cross(right, direction);

	//Moving 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= direction * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right* speed* deltaTime;
	}

	// Up and down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * speed * deltaTime;
	}

	float FoV = initialFoV;

	ProjectionMatrix = perspective(
		FoV
		, window_width / window_height
		, 0.1f
		, 1000.0f
		);

	ViewMatrix = lookAt(
		position
		, position + direction
		, up
		);

	lastTime = currentTime;
}