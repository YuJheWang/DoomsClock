#version 430 core

in vec2 uv2Fs;
out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex, uv2Fs);
    if (color.a < 0.1) discard;
    color.a *= 0.5;
}
