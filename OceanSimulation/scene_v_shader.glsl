#version 420 core

layout(location = 0) in vec3 vertexPosition_modelspace;

layout (std140, binding = 0) uniform MVP2{
	mat4 mvp;
};

void main()
{
	gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
	// gl_Position = vec4(vertexPosition_modelspace.x, vertexPosition_modelspace.y, 0, 1);
	// gl_PointSize =5;
	// UV = vertexUV;
}
