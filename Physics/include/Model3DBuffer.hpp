#pragma once

#include "VertexBuffer.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <memory>


class Model3DBuffer{	
private:
	VAO vao;
	VBO vbo, ibo;
	VertexBuffer vBuffer;
public:
	void draw();
	void load(const std::string& file);
	void loadFromMemory(const VertexBuffer& buffer);
	

	static std::shared_ptr<Model3DBuffer> axis(const glm::vec3& center, const glm::vec3& size);
};

	typedef std::shared_ptr<Model3DBuffer> Model3DBufferPtr;
