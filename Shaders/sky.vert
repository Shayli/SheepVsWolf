#version 410 core

#define POSITION 0

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec4 Color;

out block
{
	vec4 Normal;
    vec2 Texcoord;
} Out;

void main()
{  
    Out.Texcoord = UV;
    Out.Normal = vec4(normalize(Normal), 1);
    gl_Position = vec4(Position,1);
}