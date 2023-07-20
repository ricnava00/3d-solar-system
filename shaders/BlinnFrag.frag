#version 450

layout(set = 0, binding = 1, std140) uniform GlobalUniformBufferObject
{
    vec3 selector;
    vec3 lightDir;
    vec4 lightColor;
    vec3 eyePos;
} gubo;

layout(set = 0, binding = 2) uniform sampler2D tex;

layout(location = 1) in vec3 fragNorm;
layout(location = 0) in vec3 fragPos;
layout(location = 2) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

void main()
{
    vec3 N = normalize(fragNorm);
    vec3 V = normalize(gubo.eyePos - fragPos);
    vec3 L = normalize(gubo.lightDir);
    vec3 albedo = texture(tex, fragUV).xyz;
    vec3 MD = albedo * 0.89999997615814208984375;
    vec3 MS = vec3(1.0);
    vec3 MA = albedo;
    vec3 LA = vec3(0.100000001490116119384765625);
    float gamma = 180.0;
    outColor = vec4(((clamp(((MD * clamp(dot(L, N), 0.0, 1.0)) + (MS * pow(clamp(dot(N, normalize(L + V)), 0.0, 1.0), gamma))) + (LA * MA), vec3(0.0), vec3(1.0)) * gubo.selector.x) + ((vec3(0.5) + (N * 0.5)) * gubo.selector.y)) + (vec3(1.0, fragUV) * gubo.selector.z), 1.0);
}

