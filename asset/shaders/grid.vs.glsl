#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in float colorRatio;

uniform vec2 position;

uniform mat4 translate;
uniform mat4 viewMat;
uniform mat4 projMat;

out VSOUTGSIN
{
    vec3 color;
} vs_out;


void main()
{
    vec2 center = vec2(13.5, 13.5);
    float d = distance(center, position);
    float blueValue = 1.0 / (1.0 + exp(-d / (23.5 * sqrt(2))));
    vs_out.color = vec3(0.0, 1.0 - 0.75 * blueValue, blueValue)/* colorRatio*/;
    gl_Position = projMat * viewMat * translate * vec4(vPos, 1.0);
}
