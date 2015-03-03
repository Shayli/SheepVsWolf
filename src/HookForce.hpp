#pragma once

#include <Force.hpp>

class HookForce : public Force{
	float m_fK;
	float m_fL;

public:
	HookForce(const ParticlePtr& a, const ParticlePtr& b, float stiffness, float length) : Force(a,b), m_fK(stiffness), m_fL(length){};
	void update(float dt){
		applyHookForce(m_A, m_B, m_fK, m_fL, dt);
	}

	static void applyHookForce(const ParticlePtr& a, const ParticlePtr& b, float fK, float fL, float dt){
		glm::vec3 dir = b->getPosition() - a->getPosition();
		float d = glm::length(dir);
		glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
		a->addForce(f);
		b->addForce(-f);
	}
};
