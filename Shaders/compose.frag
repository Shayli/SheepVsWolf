#version 410 core

in block
{
    vec2 Texcoord;
} In;
uniform sampler2D TextureChannel0;
uniform sampler2D TextureChannel1;

layout(location = 0, index = 0) out vec4 Color;

void main(void)
{
	vec3 color = texture(TextureChannel0, In.Texcoord).rgb;
	vec3 light = texture(TextureChannel1, In.Texcoord).rgb;
    Color = vec4(color*light, 1.0);
}