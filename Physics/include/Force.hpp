#pragma once 

#include "Particle.hpp"

class Force
{
public:
	Force(const ParticlePtr& a, const ParticlePtr& b): m_A(a), m_B(b){};
	virtual ~Force() = default;

	virtual void update(float dt) = 0;

protected:
	ParticlePtr m_A;
	ParticlePtr m_B;
};

typedef std::shared_ptr<Force> ForcePtr;