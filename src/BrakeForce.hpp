#pragma once

#include <Force.hpp>

class BrakeForce : public Force{
	float m_fV;

public:
	BrakeForce(const ParticlePtr& a, const ParticlePtr& b, float brake) : Force(a,b), m_fV(brake){};

	void update(float dt){
		applyBrakeForce(m_A, m_B, m_fV, dt);
	}

	static void applyBrakeForce(const ParticlePtr& a, const ParticlePtr& b, float brake, float dt){
		glm::vec3 dV = b->getVelocity() - a->getVelocity();
		glm::vec3 f(0,0,0);
		if(glm::length(dV) >= 0.001f)
			f = brake * (dV/dt);
		a->addForce(f);
		b->addForce(-f);
	}
};
