#version 430 core

in GSOUTFSIN
{
    vec3 fscolor;
} fs_in;
out vec4 fsColor;

void main()
{
    fsColor = vec4(fs_in.fscolor, 1.0);
}
