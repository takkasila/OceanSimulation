#version 420 core

layout(location = 0) in vec3 v_pos_modelspace;
layout(location = 1) in vec3 normal;

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
  vec4 newPos = vec4(v_pos_modelspace.x,
    sin((v_pos_modelspace.x + v_pos_modelspace.z) + time * time_magnitude),
    v_pos_modelspace.z, 1);
  gl_Position = projection * view * model * newPos;
  vs_out.normal = normal;
}
