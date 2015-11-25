#version  420 core

out vec4 color;

uniform bool isUseColor;	// is use color

in VS_OUT
{
	vec3 color;
	vec3 normal;
} fs_in;

void main()
{
	if(isUseColor)
		color = vec4(fs_in.color, 1);
	else
		color = vec4(1, 1, 1, 1);
}
