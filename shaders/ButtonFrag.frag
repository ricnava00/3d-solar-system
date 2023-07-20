#version 450

layout(set = 0, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float hover;
layout(location = 2) in float selected;

layout(location = 0) out vec4 outColor;

void main()
{
	float offBrightness = 0.5;
	float hoverBrightness = 1.5;
	vec4 Tx = texture(texSampler, fragTexCoord);
	float b = offBrightness+(1-offBrightness)*(selected+hover*(1-selected))+hover*(hoverBrightness-1);
	outColor = texture(texSampler, fragTexCoord)*vec4(b,b,b,1);
}

