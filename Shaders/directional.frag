#version 410 core

in block
{
	vec2 TexCoord;
} In;

struct DirLight {
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

uniform DirLight Light;
uniform mat4 WorldToLightScreen;
uniform mat4 ScreenToWorld;

uniform sampler2D TextureChannel0; // Normal buffer
uniform sampler2D TextureChannel1; // Normal buffer

layout(location = 0) out vec4 lightColor;

float toon(float f)
{
	if(f < 0.2)
		return 0.2;
	if(f < 0.5)
		return 0.5;
	if(f < 0.8)
		return 0.8;
	return 1;
}
void main(){
	vec3 l = normalize(Light.Direction);
	vec3 n = texture(TextureChannel0, In.TexCoord).rgb;
	
	float ndotl = toon(clamp(dot(n, l),0,1));
	lightColor = vec4(Light.Color*ndotl*Light.Intensity, 1);
}