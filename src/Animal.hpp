#pragma once

#include <Particle.hpp>
#include "Behavior/Behavior.hpp"
#include <iostream>

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

		auto d = glm::normalize(m_velocity);
		auto c = acos(glm::dot(d,{0,0,1}))*180/M_PI;
		m_model->setRotation({0,c,0});
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