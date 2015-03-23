#include "Particle.hpp"

Particle::Particle(float mass) : m_mass(mass), m_velocity(glm::vec3(0,0,0)), m_force(glm::vec3(0,0,0)), m_model(new Model3D){}

glm::vec3 Particle::getVelocity() const{
	return m_velocity;
}
void Particle::setVelocity(const glm::vec3& v){
	m_velocity = v;
}

glm::vec3 Particle::getForce() const{
	return m_force;
}
void Particle::addForce(const glm::vec3& f){
	m_force += f;
}
void Particle::resetForce(){
	m_force = glm::vec3(0,0,0);
}

float Particle::getMass() const{
	return m_mass;
}

glm::vec3 Particle::getPosition() const{
	return m_model->getPosition();
}
void Particle::setPosition(const glm::vec3& p){
	m_model->setPosition(p);
}

void Particle::setRotation(const glm::vec3& r){
	m_model->setRotation(r);
}

void Particle::setScale(const glm::vec3& s){
	m_model->setScale(s);
}


Model3DPtr Particle::getModel() const{
	return m_model;
}
void Particle::setModel(const Model3DPtr& m){
	m_model = m;
}

void Particle::update(float dt){
	setVelocity(m_velocity+(dt *(m_force/m_mass)));
	setPosition(getPosition()+(dt * m_velocity));
	resetForce();
}