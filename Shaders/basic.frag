#version 410 core

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Depth;

in block
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord;
	vec4 Color;
} In;

void main()
{
	Color = In.Color;
	Normal = In.Normal;
	Position = In.Position;
	Depth = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1);
}