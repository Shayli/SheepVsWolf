#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

out block
{
	vec2 TexCoord;
} Out;

void main(){
	Out.TexCoord = uv;
	gl_Position = vec4(position,1);
}