#pragma once

#include <Particle.hpp>
#include "Behavior/Behavior.hpp"

class Animal : public Particle
{
public:
	Animal(int life, float ms, float mass, float range): Particle(mass), m_life(life), m_moveSpeed(ms), m_behav(nullptr), m_range(range){
		m_direction = glm::vec3(glm::linearRand(-1.f, 1.f),0,glm::linearRand(-1.f, 1.f));
	}
	virtual ~Animal() = default;

	int getLife(){
		return m_life;
	}

	void setLife(int life){
		m_life = life;
	}

	float getMoveSpeed(){
		return m_moveSpeed;
	}

	void setMoveSpeed(float ms){
		m_moveSpeed = ms;
	}

	void update(float dt){
		if(m_behav)
			m_behav->update(*this, dt);
		if(glm::length(m_velocity) > m_moveSpeed)
			m_velocity = glm::normalize(m_velocity)*m_moveSpeed;
	}

	void setBehavior(Behavior* b){
		m_behav = b;
	}

	float getRange(){
		return m_range;
	}

	glm::vec3 getDirection(){
		return m_direction;
	}

	void setDirection(glm::vec3 dir){
		m_direction = dir;
	}

private:
	int m_life; 
	float m_moveSpeed;
	Behavior* m_behav; 
	float m_range;
	glm::vec3 m_direction;
};

typedef std::shared_ptr<Animal> AnimalPtr;