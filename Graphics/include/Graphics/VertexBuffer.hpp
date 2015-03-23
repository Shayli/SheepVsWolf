#pragma once 
#include <Graphics/glm.hpp>
#include <vector>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 color;

};

struct Triangle {
	unsigned short v1, v2, v3;
};

class VertexBuffer{
private:
	std::vector<Vertex> vertexList;
	std::vector<Triangle> triangleList;

public:
	void addVertex(const glm::vec3& position, const glm::vec3& normal = glm::vec3(0,0,0), const glm::vec2& uv = glm::vec2(0,0), const glm::vec4& color = glm::vec4(1,1,1,1)){
		vertexList.push_back({position, normal, uv, color});
	}

	void addTriangle(unsigned short v1, unsigned short v2, unsigned short v3){
		triangleList.push_back({v1, v2, v3});
	}

	size_t getVerticesCount() const{
		return vertexList.size();
	}

	size_t getTrianglesCount() const{
		return triangleList.size();
	}

	Vertex* getVertices(){
		return vertexList.data();
	}

	Triangle* getTriangles(){
		return triangleList.data();
	}
};