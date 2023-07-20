#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform GlobalUniformBufferObject {
	vec3 lightPos;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

layout(binding = 2) uniform sampler2D tex;

vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, float sigma) {
	vec3 D = Md * clamp(dot(L, N), 0, 1);
	return D;
}

const float beta = .2f;
const float g = 10;

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	
	vec3 lightDir = normalize(gubo.lightPos - fragPos);
	vec3 lightColor = pow(g/length(gubo.lightPos-fragPos),beta)*gubo.lightColor.rgb;

	vec3 DiffSpec = BRDF(EyeDir, Norm, lightDir, texture(tex, fragUV).rgb, 0.0f);
	vec3 Ambient = texture(tex, fragUV).rgb * .03f;
	
	outColor = vec4(clamp(0.97 * (DiffSpec) * lightColor.rgb + Ambient,0.0,1.0), 1.0f);
}
