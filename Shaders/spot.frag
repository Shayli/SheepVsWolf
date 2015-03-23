#version 410 core

in block
{
    vec2 TexCoord;
} In;

uniform sampler2D TextureChannel0;
uniform sampler2D TextureChannel1;
uniform sampler2D TextureChannel2;
uniform sampler2DShadow ShadowBuffer;

uniform vec3 Eye;
uniform mat4 ScreenToWorld;

uniform float Intensity;
uniform vec3 Color;
uniform vec3 Position;
uniform vec3 Direction;
uniform float Angle;

uniform mat4 WorldToLightScreen;


#define M_PI 3.1415926535897932384626433832795
#define DEG2RAD  2*M_PI / 360.0

layout(location = 0) out vec4 FragColor;

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

vec4 spotLight()
{
	// Read gbuffer values
	//vec4 colorBuffer = texture(ColorBuffer, In.Texcoord).rgba;

	// Unpack values stored in the gbuffer
	vec3 n = texture(TextureChannel0, In.TexCoord).rgb;
	vec3 pos = texture(TextureChannel1, In.TexCoord).rgb;


	float d = max(length(Position-pos), 1.0);
	vec3 v = normalize(Eye-pos);
	vec3 l = normalize(Position-pos);
	vec3 h = normalize(l+v);
	float ndotl = clamp(dot(n, l),0,1);
	float ndoth = clamp(dot(n, h),0,1);

	float half = Angle/2;
	vec3 dir = normalize(Direction);
	float angle = acos(dot(dir,-l))*180/M_PI;
	float falloff = 0;
	if(angle < 10)
	//{
		falloff = 1;
		//falloff = pow((cos(angle*DEG2RAD)-cos(Angle*DEG2RAD))/(cos(15*DEG2RAD) -cos(Angle*DEG2RAD)),4);
	//}

	vec4 color = vec4(Color,1) * falloff * Intensity;// * ndotl * Intensity;
	return vec4(clamp(color,0,1));
}

void main(void)
{
	/*float depth = texture(DepthBuffer, In.Texcoord).r;
	// Convert texture coordinates into screen space coordinates
	vec2 xy = In.Texcoord * 2.0 -1.0;
	// Convert depth to -1,1 range and multiply the point by ScreenToWorld matrix
	vec4 wP = vec4(xy, depth * 2.0 -1.0, 1.0) * ScreenToWorld;
	// Divide by w
	vec3 pos = vec3(wP.xyz / wP.w);

	vec4 wlP = WorldToLightScreen * vec4(pos, 1.0);
	vec3 lP = vec3(wlP/wlP.w) * 0.5 + 0.5;

*/
	float shadowDepth = 1;
	/*
	//shadowDepth = textureProj(ShadowBuffer, vec4(lP.xy, lP.z - 0.005, 1.0), 0.0);

	const int SampleCount = 1;
	const float samplesf = SampleCount;
	for (int i=0;i<SampleCount;i++)
	{
	    int index = int(samplesf*random(vec4(gl_FragCoord.xyy, i)))%SampleCount;
	    float d = 0.1f;
	    shadowDepth += textureProj(ShadowBuffer, vec4(lP.xy + poissonDisk[index]/(1000.0 * 1.f/d), lP.z -0.005, 1.0), 0.0) / samplesf;
	}*/

	//float shadowDepth = texture(ShadowBuffer, lP.xy).r;
	FragColor = spotLight()*shadowDepth;	

	//FragColor = vec4(diff,0,0,1);//spotLight();
}