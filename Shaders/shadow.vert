#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;

out block
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord;
	vec4 Color;
} Out;

void main(){
	Out.Color = color;
	Out.Position = modelMatrix*vec4(position, 1.0);
	Out.Normal = normalMatrix*normalize(vec4(normal,1));
	Out.TexCoord = uv;
	gl_Position =  mvpMatrix*vec4(position,1);
}