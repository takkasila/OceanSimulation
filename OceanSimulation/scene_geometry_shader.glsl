#version 420 core

layout (points) in;
layout (points, max_vertices = 1) out;

in VS_OUT
{
  vec3 v_position;
} gs_in[];
void main()
{
  gl_Position = vec4(1, 1, 1, 1);
  EmitVertex();
  EndPrimitive();
}
