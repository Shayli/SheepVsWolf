#pragma once

#include "glm.hpp"
#include "Model3D.hpp"

class Particle
{
public:
	Particle(float mass);
	virtual ~Particle() = default;

	glm::vec3 getVelocity() const;
	void setVelocity(const glm::vec3& v);

	glm::vec3 getForce() const;
	void addForce(const glm::vec3& f);
	void resetForce();

	float getMass() const;

	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3& p);
	void setRotation(const glm::vec3& r);
	void setScale(const glm::vec3& s);

	Model3Dptr getModel() const;
	void setModel(const Model3Dptr& m);

	void update(float dt);
	
protected:
	Model3Dptr m_model;
	glm::vec3 m_velocity;
	glm::vec3 m_force;
	float m_mass;
};

typedef std::shared_ptr<Particle> ParticlePtr;