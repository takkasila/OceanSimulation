#version  420 core
in vec2 UV;
out vec4 color;

//uniform sampler2D textureSampler;
void main()
{
	//color = texture2D(textureSampler, UV).rgb;
	if(gl_FragCoord.x < 600)
		color = vec4(1, 1, 1, 1);
	else
		color = vec4(0, 0, 0, 1);
}
