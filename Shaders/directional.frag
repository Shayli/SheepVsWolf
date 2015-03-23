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

uniform sampler2D TextureChannel0;
uniform sampler2D TextureChannel1;
uniform sampler2D TextureChannel2;
uniform sampler2D TextureChannel3;
layout(location = 0) out vec4 lightColor;


vec2 poissonDisk[16] = vec2[](
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.99706507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.19984126, 0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
);
float random(vec4 seed)
{
    float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
} 

float toon(float f)
{
	return f;
	if(f < 0.2)
		return 0.1;
	if(f < 0.5)
		return 0.3;
	if(f < 0.8)
		return 0.6;
	return 1;
}
void main(){
	vec3 l = normalize(Light.Direction);
	vec3 n = texture(TextureChannel0, In.TexCoord).rgb;
	vec3 p = texture(TextureChannel1, In.TexCoord).rgb;
	float depth = texture(TextureChannel2, In.TexCoord).r;

	// Convert texture coordinates into screen space coordinates
	/*vec2 xy = In.TexCoord * 2.0 -1.0;
	// Convert depth to -1,1 range and multiply the point by ScreenToWorld matrix
	vec4 wP = vec4(xy, depth * 2.0 -1.0, 1.0) * ScreenToWorld;
	// Divide by w
	vec3 pos = vec3(wP.xyz / wP.w);*/
	vec4 wlP = WorldToLightScreen*vec4(p, 1);
	vec3 lP = vec3(wlP/wlP.w) * 0.5 + 0.5;
	float shadowDepth = 1;
	//if(texture(TextureChannel3, lP.xy).z < lP.z)
	//	shadowDepth = 0.2;

	float ndotl = toon(clamp(dot(n, l),0,1));


//	if(lP.z - shadowDepth > 0)
//		shadowDepth = 0;
//	else
//		shadowDepth = 1;
	/*float shadowDepth = 0.0;
	
	const int SampleCount = 1;
	const float samplesf = SampleCount;
	for (int i=0;i<SampleCount;i++)
	{
	    int index = int(samplesf*random(vec4(gl_FragCoord.xyy, i)))%SampleCount;
	    float d = 0.1f;
	    shadowDepth += textureProj(TextureChannel3, vec4(lP.xy + poissonDisk[index]/(1000.0 * 1.f/d), lP.z -0.005, 1.0), 0.0) / samplesf;
	}*/

	//lightColor = vec4(shadowDepth, shadowDepth, shadowDepth,1);
	lightColor = vec4(Light.Color*ndotl*Light.Intensity*shadowDepth, 1);
}