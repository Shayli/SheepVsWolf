#pragma once 

#include "Animal.hpp"


class Wolf : public Animal
{
public:
	Wolf() : Animal(300, 1.5f, 10, 10){};
	
};