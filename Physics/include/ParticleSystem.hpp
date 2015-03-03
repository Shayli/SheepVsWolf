#pragma once

#include "Force.hpp"

class ParticleSystem
{
public:
	void addParticle(const ParticlePtr& p){
		m_particleList.push_back(p);
	}

	void removeParticle(const ParticlePtr& p){
		for(auto it = m_particleList.begin(); it != m_particleList.end(); ++it){
	        if((*it).get() == p.get()){
	            m_particleList.erase(it);
	            break;
	        }
	    }
	}

	void addForce(const ForcePtr& f){
		m_forceList.push_back(f);
	}
	void update(float dt){
		for(auto& f : m_forceList){
			f->update(dt);
		}
		for(auto& p : m_particleList){
			p->update(dt);
		}
	}

private:
	std::vector<ParticlePtr> m_particleList;
	std::vector<ForcePtr> m_forceList;
};