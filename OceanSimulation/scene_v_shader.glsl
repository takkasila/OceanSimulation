#version 420 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

// uniform mat4 MVP;
layout (std140, binding = 0) uniform MVP2{
	mat4 mvp;
};

void main()
{
	gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
	// gl_PointSize = gl_Position.z * gl_VertexID;
	// UV = vertexUV;
}
