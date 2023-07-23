#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform GlobalUniformBufferObject {
	vec2 selector;
	vec3 lightPos;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

layout(binding = 2) uniform sampler2D tex;

void main() {
	float lod = textureQueryLod(tex, fragUV).x;
	vec4 outColorDef = vec4(textureLod(tex, fragUV, lod).rgb, 1.0f);
	vec4 outColorLod = vec4(floor(lod)/6, 1.f-floor(lod)/6, 0.f, 1.0f);
	outColor = outColorDef * gubo.selector.x + outColorLod * gubo.selector.y;
}
