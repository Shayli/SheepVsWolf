#include "SheepIdleBehavior.hpp"
#include "../Entity.hpp"
#include "../FleeForce.hpp"
#include "../Map.hpp"

void SheepIdleBehavior::update(Animal& a, float dt){
	glm::vec3 dir(glm::ballRand(1.f));
	dir.y = 0;
	

	float bestrange = a.getRange()+1;
	auto& near = m_Entity.wolves[0];
	for(auto& s : m_Entity.wolves){
		float r = glm::length(s->getPosition() - a.getPosition());
		if(r < bestrange){
			near = s;
			bestrange = r;
		}
	}

	if(bestrange < a.getRange()) {
		FleeForce::apply(a, *near.get());
	}else{
		a.addForce(glm::normalize(dir)*0.1f);
	}

	if(glm::length(near->getPosition()- a.getPosition()) < 1.f){
		a.setLife(a.getLife()-1);
		//std::cout << "Life : " << a.getLife() << std::endl;
	}
}
