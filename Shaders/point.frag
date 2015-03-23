#version 410 core

in block
{
    vec2 TexCoord;
} In;

uniform sampler2D TextureChannel0;
uniform sampler2D TextureChannel1;

struct SpotLight {
	vec3 Position;
	vec3 Color;
	float Intensity;
};

uniform SpotLight Light;

layout(location = 0) out vec4 FragColor;
 
void main(void)
{
	// Unpack values stored in the gbuffer
	vec3 p = texture(TextureChannel1, In.TexCoord).rgb;
	vec3 n = texture(TextureChannel0, In.TexCoord).rgb;
	vec3 l = normalize(Light.Position-p);

	float d = max(length(Light.Position-p), 1.0);
	float ndotl = clamp(dot(n, l),0,1);

	vec3 color = Light.Color * ndotl*1/(d*d) * Light.Intensity;
	FragColor = vec4(clamp(color,0,1),1);
}