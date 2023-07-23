#version 450

layout(set = 0, binding = 0, std140) uniform UniformBufferObject
{
    float Ar;
} ubo;

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec2 offset;
layout(location = 3) in vec2 scale;
layout(location = 4) in float visible;
layout(location = 5) in float enabled;
layout(location = 6) in float hover;
layout(location = 7) in float selected;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float outEnabled;
layout(location = 2) out float outHover;
layout(location = 3) out float outSelected;

void main()
{
    gl_Position = visible*vec4(inPos/vec2(ubo.Ar,1.0)*scale+offset, 0, 1.0);
    fragTexCoord = vec2(inUV.x*scale.x/scale.y,inUV.y);
    outEnabled = enabled;
    outHover = hover;
    outSelected = selected;
}

