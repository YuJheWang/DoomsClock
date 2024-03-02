#version 430 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

out vec2 uv2Fs;

uniform mat4 MVP;

void main()
{
    uv2Fs = uv;

    gl_Position = MVP * vec4(pos, 0.0, 1.0);
}
