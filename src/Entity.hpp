#pragma once

#include "Sheep.hpp"
#include "Wolf.hpp"

struct Entity
{
    std::vector<std::shared_ptr<Sheep>> sheeps;
    std::vector<std::shared_ptr<Wolf>> wolves;
};