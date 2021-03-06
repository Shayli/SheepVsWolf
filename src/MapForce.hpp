#pragma once

#include <Force.hpp>
#include "Animal.hpp"
#include "Map.hpp"


class MapForce : public Force{

public:
	MapForce(const AnimalPtr& a) : Force(a, nullptr){};
	void update(float){
		glm::vec3 pos = m_A->getPosition();
		static const glm::vec3 right(32, 0,0);
		static const glm::vec3 top(0,0,32);
		static const float fK = 10;
		static const float fL = 2;
		{
			// Right side
			glm::vec3 dir = right-glm::vec3(pos.x,0,0);
			float d = glm::length(dir);
			if(d > 0.001f && d < fL) {
				glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
				m_A->addForce(f);
			}
		}
		{
			// Left side
			glm::vec3 dir = (-right-glm::vec3(pos.x,0,0));
			float d = glm::length(dir);
			if(d > 0.001f  && d < fL) {
				glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
				m_A->addForce(f);
			}
		}
		{
			// Top side
			glm::vec3 dir = top-glm::vec3(0,0,pos.z);
			float d = glm::length(dir);
			if(d > 0.001f && d < fL) {
				glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
				m_A->addForce(f);
			}
		}
		{
			// Bottom side
			glm::vec3 dir = (-top-glm::vec3(0,0,pos.z));
			float d = glm::length(dir);
			if(d > 0.001f  && d < fL) {
				glm::vec3 f = fK * (1 - (fL/std::max(d, 0.001f))) * dir;
				m_A->addForce(f);
			}
		}

		/*if(pos.x > Map::width/2){
	        m_A->addForce(glm::vec3(-Map::width/2, pos.y, pos.z));
	    }
	    if(pos.x < -Map::width/2){
	        m_A->addForce(glm::vec3(Map::width/2, pos.y, pos.z));
	    }
	    if(pos.z > Map::height/2){
	        m_A->addForce(glm::vec3(pos.x, pos.y, -Map::height/2));
	    }
	    if(pos.z < -Map::height/2){
	        m_A->addForce(glm::vec3(pos.x, pos.y, Map::height/2));
	    }*/
	}
};
