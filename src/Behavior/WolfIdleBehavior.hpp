#pragma once

#include <iostream>

#include "Behavior.hpp"
#include "../Animal.hpp"

class WolfIdleBehavior : public Behavior
{
public:
	WolfIdleBehavior(Entity& e) : Behavior(e){}
	void update(Animal& a, float dt);
};