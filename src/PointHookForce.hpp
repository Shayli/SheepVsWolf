#pragma once

#include <Force.hpp>

class PointHookForce : public Force{
	float m_fK;
	float m_fL;
	glm::vec3 m_point;

public:
	PointHookForce(const ParticlePtr& a, const glm::vec3& point, float stiffness, float length) : 
		Force(a,nullptr), m_fK(stiffness), m_fL(length), m_point(point)
	{}

	void update(float dt){
		applyPointHookForce(m_A, m_point, m_fK, m_fL, dt);
	}

	static void applyPointHookForce(const ParticlePtr& a, const glm::vec3& p, float fK, float fL, float dt){
		glm::vec3 dir = p - a->getPosition();
		float d = glm::length(dir);
		//std::cout << "point " << d << " " << p << std::endl;
		if(d > fL)
			return;
		//std::cout << "point " << d << " " << p << std::endl;
		glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
		a->addForce(f);
	}
};
