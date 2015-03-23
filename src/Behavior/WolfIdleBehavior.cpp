#include "WolfIdleBehavior.hpp"
#include "../Entity.hpp"
#include "../ChaseForce.hpp"
#include "../Map.hpp"
#include <iostream>

void WolfIdleBehavior::update(Animal& a, float dt){
	float bestrange = a.getRange()+1;
	auto& near = m_Entity.sheeps[0];
	for(auto& s : m_Entity.sheeps){
		float r = glm::length(s->getPosition() - a.getPosition());
		if(r < bestrange){
			near = s;
			bestrange = r;
		}
	}

	if(bestrange < a.getRange()) {
		ChaseForce::apply(a, *near.get());
	}else{
		glm::vec3 dir = a.getVelocity();
		/*if(a.getPosition().x > Map::width/2){
            dir.x = -1;
            dir.z = glm::linearRand(-1.f, 1.f);
        }
        if(a.getPosition().x < -Map::width/2){
            dir.x = 1;
            dir.z = glm::linearRand(-1.f, 1.f);
        }
        if(a.getPosition().z > Map::height/2){
            dir.z = -1;
            dir.x = glm::linearRand(-1.f, 1.f);
        }
        if(a.getPosition().z < -Map::height/2){
            dir.z = 1;
            dir.x = glm::linearRand(-1.f, 1.f);
        }*/
//        a.setDirection(dir);
		//a.addForce(glm::normalize(dir));
        std::cout << a.getPosition()<< std::endl;
	}

}
