#version  420 core

out vec4 color;

uniform bool isUseColor;	// is use color

in VS_OUT
{
	vec3 color;
	vec3 normal;
} fs_in_vs;

in GS_OUT
{
	vec3 color;
} fs_in_gs;

void main()
{
	if(isUseColor)
		color = vec4(fs_in_vs.color, 1);
	else
		color = vec4(fs_in_gs.color, 1);
}
