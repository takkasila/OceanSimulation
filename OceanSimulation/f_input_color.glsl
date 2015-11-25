#version  420 core

out vec4 color;

uniform bool isUseColor;	// is use color

in VS_OUT
{
	vec3 color;
	vec3 normal;
	vec2 WaveDir;
} fs_in;

void main()
{
	if(isUseColor)
		color = vec4(fs_in.color, 1);
	else
		color = vec4(1, 1, 1, 1);

	if(gl_FragCoord.x < 100)
	{
		if(fs_in.WaveDir.x >= 0.4)
			color = vec4(1, 0, 0, 1);
	}

	if(gl_FragCoord.x > 1100)
	{
		if(fs_in.WaveDir.y >= 0.44)
			color = vec4(1, 0, 0, 1);
	}
}
