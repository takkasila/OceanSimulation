#version 420 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 color;
  vec3 normal;
} gs_in[];

out GS_OUT
{
	vec3 color;
}gs_out;

void GenerateLine(int index)
{
  gl_Position = gl_in[index].gl_Position;
	gs_out.color = vec3(1, 0, 0);
  EmitVertex();

  gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0) * 1;
	gs_out.color = vec3(1, 0, 0);
  EmitVertex();

  EndPrimitive();
}

void main()
{
  GenerateLine(0);
  GenerateLine(1);
  GenerateLine(2);
}
