#version 450

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;

void main()
{
	outColor = vec4(textureLod(tex, vec2(0,0), 1./0.).rgb, .4);
}