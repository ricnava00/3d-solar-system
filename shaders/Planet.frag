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

layout(binding = 2) uniform sampler2D texDay;
layout(binding = 3) uniform sampler2D texNight;

const float beta = .8;
const float g = 20;

void main() {
	float lod = textureQueryLod(texDay, fragUV).x;

	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);

	vec3 albedoDay = textureLod(texDay, fragUV, lod).rgb;
	vec3 albedoNight = textureLod(texNight, fragUV, lod).rgb;
	bool hasNight = albedoDay != albedoNight; //Yes, can be inaccurate but it's quite impossible
	
	vec3 lightDir = normalize(gubo.lightPos - fragPos);
	vec3 lightColor = clamp(pow(g/length(gubo.lightPos-fragPos),beta)*gubo.lightColor.rgb,0,1);

	float lightQuantity = clamp(dot(lightDir, Norm), 0, 1);
	float textureSelector = clamp((lightQuantity-0.5)*4+0.5, 0, 1);
	vec3 Ambient = albedoDay * textureSelector + albedoNight * (1-textureSelector);
	vec3 DiffSpec = lightQuantity * Ambient;
	
	float AmbientFactor = 0.03;
	if(hasNight)
	{
		AmbientFactor = 0.5;
	}
	
	float DiffFactor = 1 - AmbientFactor;
	
	vec4 outColorDef = vec4(clamp(DiffFactor * DiffSpec * lightColor.rgb + AmbientFactor * Ambient,0.0,1.0), 1.0f);
	vec4 outColorLod = vec4(floor(lod)/6, 1.f-floor(lod)/6, 0.f, 1.0f);
	outColor = outColorDef * gubo.selector.x + outColorLod * gubo.selector.y;
}
