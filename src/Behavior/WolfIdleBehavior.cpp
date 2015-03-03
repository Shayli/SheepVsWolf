#include "WolfIdleBehavior.hpp"
#include "../Entity.hpp"
#include "../ChaseForce.hpp"
#include "../Map.hpp"

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
		glm::vec3 dir = a.getDirection();
		if(a.getPosition().x > Map::width/2){
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
        }
        a.setDirection(dir);
		a.addForce(dir);
	}
	glm::vec3 pos = a.getPosition();
	if(pos.x > Map::width/2){
        a.setPosition(glm::vec3(Map::width/2, pos.y, pos.z));
        
    }
    if(pos.x < -Map::width/2){
        a.setPosition(glm::vec3(-Map::width/2, pos.y, pos.z));
       // a.setDirection(glm::vec3(glm::linearRand(0.f, 1.f),0,glm::linearRand(-1.f, 1.f)));
    }
    if(pos.z > Map::height/2){
        a.setPosition(glm::vec3(pos.x, pos.y, Map::height/2));
        //a.setDirection(glm::vec3(glm::linearRand(-1.f, 1.f),0,glm::linearRand(-1.f, 0.f)));
    }
    if(pos.z < -Map::height/2){
        a.setPosition(glm::vec3(pos.x, pos.y, -Map::height/2));
        //a.setDirection(glm::vec3(glm::linearRand(-1.f, 1.f),0,glm::linearRand(0.f, 1.f)));
    }
}
