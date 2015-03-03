#version 330 core

layout(location = 0) in vec3 aVertexPosition;

uniform vec3 uParticleColor;


void main() {
    gl_Position = vec4(uParticleColor, 1.0);
}