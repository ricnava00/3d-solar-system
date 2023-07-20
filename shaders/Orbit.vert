#version 450

layout(set = 0, binding = 0, std140) uniform UniformBufferObject
{
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 fragColor;
layout(location = 1) in vec3 inColor;

void main()
{
    gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
    fragColor = inColor;
}