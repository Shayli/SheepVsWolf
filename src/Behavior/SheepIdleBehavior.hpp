#pragma once

#include <iostream>

#include "Behavior.hpp"
#include "../Animal.hpp"

class SheepIdleBehavior : public Behavior
{
public:
	SheepIdleBehavior(Entity& e): Behavior(e){}
	void update(Animal& a, float dt);
};