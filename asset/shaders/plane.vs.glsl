#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

uniform mat4 projMat;
uniform mat4 viewMat;

out vec2 fsUV;

void main()
{
    fsUV = uv;
    gl_Position = projMat * viewMat * vec4(pos, 1.0);
}
