#version 430 core

in vec2 uv2Fs;

out vec4 fsColor;

uniform sampler2D tex;

void main()
{
    //fsColor = vec4(0.0471, 0.5725, 0.302, 1.0);
    fsColor = texture(tex, uv2Fs);
    if (fsColor.a < 0.1) discard;
}
