#pragma once

#include <Graphics/Model3DBuffer.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Material.hpp>

#include <memory>

#define DEG2RAD (float)(M_PI/180.f)

enum TextureChannel {
	TextureChannel_0,
	TextureChannel_1,
	TextureChannel_2,

	TextureChannel_Max,
};


class Model3D{
private:
	std::vector<Model3DBufferPtr> modelBuffers;
	Shader* shader;
	glm::mat4 modelMatrix, normalMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::array<MaterialPtr, TextureChannel_Max> materials;
public:
	Model3D();
	void copyTo(Model3D& o);

	void draw();
	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();
	Shader* getShader();
	void setShader(Shader* s);
	void addModelBuffer(const Model3DBufferPtr& mBuffer);

	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);
	void setScale(const glm::vec3& scal);
	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;

	uint32_t getModelBufferCount() const;
	Model3DBufferPtr& getModelBuffer(uint32_t i);
	void removeModelBuffer(uint32_t i);

	std::array<MaterialPtr, TextureChannel_Max>& getMaterials();
	void setMaterial(const MaterialPtr& m, TextureChannel channel);
};

typedef std::shared_ptr<Model3D> Model3DPtr;