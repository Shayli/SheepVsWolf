#pragma once 
#include <Graphics/glm.hpp>
#include <vector>
#include <cstdint>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 color;

};

struct Triangle {
	uint32_t v1, v2, v3;
	Triangle() : Triangle(0,0,0) {}
	Triangle(uint32_t a, uint32_t b, uint32_t c) : v1(a), v2(b), v3(c) {}
};

class VertexBuffer{
private:
	std::vector<Vertex> vertexList;
	std::vector<Triangle> triangleList;

public:
	void addVertex(const glm::vec3& position, const glm::vec3& normal = glm::vec3(0,0,0), const glm::vec2& uv = glm::vec2(0,0), const glm::vec4& color = glm::vec4(1,1,1,1)){
		vertexList.push_back({position, normal, uv, color});
	}

	void addTriangle(uint32_t v1, uint32_t v2, uint32_t v3){
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

	std::vector<Vertex>& getVerticesArray() {
		return vertexList;
	}

	std::vector<Triangle>& getTrianglesArray() {
		return triangleList;
	}

	Vertex& getVertex(uint32_t id)
	{
		return vertexList[id];
	}

	void clear()
	{
		triangleList.clear();
		vertexList.clear();
	}
};