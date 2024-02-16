#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VSOUTGSIN
{
    vec3 color;
} gs_in[];

out GSOUTFSIN
{
    vec3 fscolor;
} gs_out;

void main()
{
    vec3 lightRay = vec3(0, -1.732 / 2.0, 1.0 / 2.0);

    vec3 u = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 v = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(u, v));
    float ratio = abs(dot(normal, lightRay));
    
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        gs_out.fscolor = gs_in[i].color * ratio;
        EmitVertex();
    }
}
