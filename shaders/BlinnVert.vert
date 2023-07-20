#version 450

layout(set = 0, binding = 0, std140) uniform UniformBufferObject
{
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNorm;
layout(location = 1) in vec3 inNorm;
layout(location = 2) out vec2 outUV;
layout(location = 2) in vec2 inUV;

void main()
{
    gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
    fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
    fragNorm = (ubo.nMat * vec4(inNorm, 0.0)).xyz;
    outUV = inUV;
}

