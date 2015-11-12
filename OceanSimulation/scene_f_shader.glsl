#version  420 core

in vec3 fColor;
out vec4 color;
//uniform sampler2D textureSampler;
void main()
{
	color = vec4(fColor, 1);
	// color = vec4(1, 1, 1, 1);
}
