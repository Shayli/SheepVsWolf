#version 410 core

in block
{
    vec2 Texcoord;
} In;

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Depth;

void main(void)
{
    Color = vec4(30/255.0, 128/255.0, 204/255.00, 1.0);
    Normal = vec4(1,1,1,0);
    Position = vec4(0,0,0,0);
    Depth = vec4(999.f,999.f,999.f,1);
    gl_FragDepth = 999.f;
}