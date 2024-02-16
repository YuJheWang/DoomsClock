#version 430 core

uniform sampler2D image;

in vec2 fsUV;

out vec4 fsColor;

void main()
{
    fsColor = texture(image, fsUV);
}
