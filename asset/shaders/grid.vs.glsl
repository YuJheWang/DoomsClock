#version 430 core

layout(location = 0) in vec3 vPos;

uniform vec2 mousePos;
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
    mat4 MVP = projMat * viewMat * translate;
    vec3 mousePos = (inverse(MVP) * vec4(mousePos, 0.0, 1.0)).xyz;
    float k = sqrt(2) * mousePos.y;
    mousePos += vec3(0.5, -sqrt(2) / 2.0, 0.5) * k;
    float heightRatio = 1.0;

    if ((mousePos.x < 1 && mousePos.z < 1) && 
        (mousePos.x >= 0 && mousePos.z >= 0))
    {
        vs_out.color = vec3(0.1451, 0.4863, 0.7294);
        heightRatio = 2.0;
    }

    vec2 center = vec2(13.5, 13.5);
    float d = distance(center, position);
    float blueValue = 1.0 / (1.0 + exp(-d / (23.5 * sqrt(2))));
    vs_out.color = vec3(0.0, 1.0 - 0.5 * blueValue, blueValue) * heightRatio;
    gl_Position = MVP * vec4(vPos.x, vPos.y * heightRatio, vPos.z, 1.0);
}
