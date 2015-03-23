#pragma once

#include <GL/glew.h>

class VAO {
	public:
		VAO() {
			glGenVertexArrays(1, &m_nGLId);
		}
	
		VAO(const VAO& other) = delete;

		virtual ~VAO() {
			glDeleteVertexArrays(1,&m_nGLId);
		}

		void bind() const
		{
			glBindVertexArray(m_nGLId);
		}
		void unbind() const
		{
			glBindVertexArray(0);
		}

		GLuint getGLId() const { return m_nGLId; } 

		VAO& operator=(const VAO& other) = delete;
	protected:
		GLuint m_nGLId;
};