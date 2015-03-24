#version 410 core

in block
{
	vec4 Normal;
    vec2 Texcoord;
} In;

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Depth;

void main(void)
{
    Color = vec4(30/255.0, 128/255.0, 204/255.00, 1.0);
    Normal = In.Normal;
    Position = vec4(0,0,0,0);
    Depth = vec4(1,1,1,1);
    gl_FragDepth = 0.9999999f;
}