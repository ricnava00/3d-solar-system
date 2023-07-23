#version 450

layout(set = 0, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float enabled;
layout(location = 2) in float hover;
layout(location = 3) in float selected;

layout(location = 0) out vec4 outColor;

vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 rgb2hsv(vec4 c)
{
	return vec4(rgb2hsv(c.rgb), c.a);
}

vec4 hsv2rgb(vec4 c)
{
	return vec4(hsv2rgb(c.rgb), c.a);
}

void main()
{
	//float offBrightness = 0.5;
	float hoverBrightness = 1.5;
	//float b = offBrightness+(1-offBrightness)*(selected+hover*(1-selected))+hover*(hoverBrightness-1);
	float b = 1 + hover * (hoverBrightness - 1);
	vec4 rgba = texture(texSampler, fragTexCoord);
	vec4 hsva = rgb2hsv(rgba);
	if(selected == 0)
	{
		hsva.x = mod(hsva.x + 0.37, 1);
	}
	hsva.y *= enabled;
	hsva.z -= 0.05 * (1 - enabled);
	outColor = hsv2rgb(hsva) * vec4(b, b, b, 1);
}

