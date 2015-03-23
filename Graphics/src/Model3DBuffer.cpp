#include <Graphics/Model3DBuffer.hpp>
#include <iostream>

void Model3DBuffer::draw(){
	vao.bind();
	ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDrawElements(GL_TRIANGLES, vBuffer.getTrianglesCount()*3, GL_UNSIGNED_SHORT, (void*)0);
	ibo.unbind(GL_ELEMENT_ARRAY_BUFFER);
	vao.unbind();
}

std::shared_ptr<Model3DBuffer> Model3DBuffer::axis(const glm::vec3& center, const glm::vec3& size){
	VertexBuffer buffer;
	buffer.addVertex(center + glm::vec3(0, 0, 0), 				glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(1,0,0,1));
	buffer.addVertex(center + glm::vec3(size.x, 0, 0), 			glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(1,0,0,1));
	buffer.addVertex(center + glm::vec3(size.x, 0, size.x/10.f),glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(1,0,0,1));

	buffer.addVertex(center + glm::vec3(0, 0, 0), 				glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,1,0,1));
	buffer.addVertex(center + glm::vec3(0, size.y, 0), 			glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,1,0,1));
	buffer.addVertex(center + glm::vec3(0, size.y, size.y/10.f),glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,1,0,1));

	buffer.addVertex(center + glm::vec3(0, 0, 0),				glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0,1,1));
	buffer.addVertex(center + glm::vec3(0, 0, size.z),			glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0,1,1));
	buffer.addVertex(center + glm::vec3(size.z/10.f, 0, size.z),glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0,1,1));

	buffer.addTriangle(0,2,1);
	buffer.addTriangle(0,1,2);

	buffer.addTriangle(3,4,5);
	buffer.addTriangle(3,5,4);

	buffer.addTriangle(6,7,8);
	buffer.addTriangle(6,8,7);

	std::shared_ptr<Model3DBuffer> model(new Model3DBuffer);
	model->loadFromMemory(buffer);
	return model;
}


void Model3DBuffer::loadFromMemory(const VertexBuffer& buffer){
	vBuffer = buffer;

	vbo.bind();
	glBufferData(GL_ARRAY_BUFFER, vBuffer.getVerticesCount()*sizeof(Vertex), vBuffer.getVertices(), GL_STATIC_DRAW);
	vbo.unbind();

	vao.bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	vbo.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
	vbo.unbind();
	vao.unbind();

	ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vBuffer.getTrianglesCount() * sizeof(unsigned short)*3, vBuffer.getTriangles(), GL_STATIC_DRAW);
	ibo.unbind(GL_ELEMENT_ARRAY_BUFFER);
}