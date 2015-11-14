#version 420 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

layout(std140, binding = 0) uniform MVP
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

out VS_OUT
{
	vec3 normal;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1);
  mat3 normalMat = mat3(transpose(inverse(view * model)));
  vs_out.normal = normalize(vec3(projection * vec4(normalMat * normal, 1.0)));
}
