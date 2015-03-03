#include "Model3D.hpp"

Model3D::Model3D() :
	shader(nullptr),
	scale(1,1,1)
{}

void Model3D::draw(){
	for(auto& m: modelBuffers)
		m->draw();
}

glm::mat4 Model3D::getModelMatrix(){
	glm::mat4 rot = glm::rotate(glm::mat4(),rotation.x*DEG2RAD, glm::vec3(1,0,0));
	rot = glm::rotate(rot,rotation.y*DEG2RAD, glm::vec3(0,1,0));
	rot = glm::rotate(rot,rotation.z*DEG2RAD, glm::vec3(0,0,1));
	glm::mat4 trans = glm::translate(glm::mat4(), position);
	glm::mat4 sca = glm::scale(glm::mat4(), scale);
	modelMatrix = trans*rot*sca;
	return modelMatrix;
}


Shader* Model3D::getShader(){
	return shader;
}

void Model3D::setShader(Shader* s){
	shader = s;
}

void Model3D::addModelBuffer(const Model3DBufferPtr& mBuffer){
	modelBuffers.push_back(mBuffer);
}

void Model3D::setPosition(const glm::vec3& pos){
	position = pos;
}
glm::vec3 Model3D::getPosition() const{
	return position;
}

void Model3D::setRotation(const glm::vec3& rot)
{
	rotation = rot;
}

void Model3D::setScale(const glm::vec3& scal){
	scale = scal;
}
