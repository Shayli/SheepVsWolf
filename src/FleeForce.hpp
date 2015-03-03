#pragma once

#include <Force.hpp>
#include "Animal.hpp"

class FleeForce : public Force{
public:
	FleeForce(const AnimalPtr& a, const AnimalPtr& b) : Force(a,b){};
	void update(float dt){
		apply(*m_A.get(), *m_B.get());
	}

	static void apply(Particle& a, Particle& b){
		glm::vec3 dir = b.getPosition() - a.getPosition();
		float d = glm::length(dir);
		Animal* a2 = dynamic_cast<Animal*>(&a);
		//Animal* b2 = dynamic_cast<Animal*>(b.get());
		if(d < a2->getRange()){
			glm::vec3 f = glm::normalize(dir); //m_fK * (1 - (m_range/std::max(d, 0.001f))) * dir;
			float v_A = a2->getMoveSpeed();
			//float v_B = b2->getMoveSpeed();
			a.addForce(-f*v_A);
			//b->addForce(f*v_B);
		}
	}
};
