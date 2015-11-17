#version 420 core

layout(location = 0) in vec3 v_pos_modelspace;
layout(location = 1) in vec3 normal;

uniform vec2 center;
layout(std140, binding = 0) uniform MVP
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

layout(std140, binding = 1) uniform TIME
{
  float time;
  float time_magnitude;
};

out VS_OUT
{
  vec3 color;
  vec3 normal;
}vs_out;

void main()
{
  float height = sin(sqrt(pow(v_pos_modelspace.x - center.x, 2) + pow(v_pos_modelspace.z - center.y, 2)) + time*time_magnitude);
  vec4 newPos = vec4(v_pos_modelspace.x, height, v_pos_modelspace.z, 1);
  gl_Position = projection * view * model * newPos;
  vs_out.normal = normal;
}
