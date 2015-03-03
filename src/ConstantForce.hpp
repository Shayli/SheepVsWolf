#pragma once

#include <Force.hpp>
#include "Animal.hpp"

class ConstantForce : public Force{

	glm::vec3 m_Force;
public:
	ConstantForce(const AnimalPtr& a, const glm::vec3& force) : Force(a, nullptr), m_Force(force){};
	void update(float){
		m_A->addForce(m_Force);
	}
};
