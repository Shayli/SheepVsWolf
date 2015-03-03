#pragma once

class Entity;
class Animal;
class Behavior
{
public:
	Behavior(Entity& e) : m_Entity(e){}
	virtual void update(Animal& a, float dt) = 0;

protected:
	Entity& m_Entity;
};