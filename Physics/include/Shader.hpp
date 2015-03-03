#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

typedef enum {
	UniformType_Mat4
} UniformType;

class Shader{
protected:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programId;

public:
	Shader();
	~Shader();
	bool loadFromFile(const std::string& vertex, const std::string& fragment);
	bool compile();
	void bind();
	void unbind();

	template<typename T>
	void send(UniformType type, const std::string& location, const T& t) {
		int loc = glGetUniformLocation(programId, location.c_str());
		if(loc == -1) {
			return;
		}
		switch(type) {
		case UniformType_Mat4:
			glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)t);
			break;
		default:
			break;
		}
	}
};