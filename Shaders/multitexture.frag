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

uniform sampler2D TextureChannel0;
uniform sampler2D TextureChannel1;
uniform sampler2D TextureChannel2;

void main()
{
	if(In.Position.y > 9)
		Color = In.Color*texture(TextureChannel0,In.TexCoord);
	else if(In.Position.y > 1)
		Color = In.Color*texture(TextureChannel1,In.TexCoord);
	else
		Color = In.Color*texture(TextureChannel2,In.TexCoord);
	
	Normal = In.Normal;
	Position = In.Position;
	Depth = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1);
}