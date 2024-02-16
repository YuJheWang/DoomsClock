#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 projMat;
uniform mat4 viewMat;

out vec2 uv2Fs;

void main()
{
    uv2Fs = uv;
    gl_Position = projMat * viewMat * translate * scale * vec4(vPos, 1.0);
}
