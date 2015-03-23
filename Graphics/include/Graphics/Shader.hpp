#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

typedef enum {
	UniformType_Mat4,
	UniformType_Vec3,
	UniformType_Float,
	UniformType_Integer
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
	void send(UniformType type, const std::string& location, const T& u) {
		T& t = const_cast<T&>(u);
		int loc = glGetUniformLocation(programId, location.c_str());
		if(loc == -1) {
			return;
		}
		switch(type) {
		case UniformType_Mat4:
			glUniformMatrix4fv(loc, 1, GL_FALSE, *reinterpret_cast<GLfloat**>(&t));
			break;
		case UniformType_Vec3:
			glUniform3fv(loc, 1, *reinterpret_cast<GLfloat**>(&t));
			break;
		case UniformType_Float:
			glUniform1f(loc, *reinterpret_cast<GLfloat*>(&t));
			break;
		case UniformType_Integer:
			glUniform1i(loc, *reinterpret_cast<GLint*>(&t));
			break;
		default:
			break;
		}
	}
};