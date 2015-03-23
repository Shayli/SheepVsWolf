#pragma once

#include <Graphics/Model3DBuffer.hpp>
#include <Graphics/Shader.hpp>

#include <memory>

#define DEG2RAD (float)(M_PI/180.f)

class Model3D{
private:
	std::vector<Model3DBufferPtr> modelBuffers;
	Shader* shader;
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:
	Model3D();
	void draw();
	glm::mat4 getModelMatrix();
	Shader* getShader();
	void setShader(Shader* s);
	void addModelBuffer(const Model3DBufferPtr& mBuffer);

	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);
	void setScale(const glm::vec3& scal);
	glm::vec3 getPosition() const;
};

typedef std::shared_ptr<Model3D> Model3DPtr;