#pragma once

#include <Graphics/glm.hpp>
#include <memory>

enum LightType {
	LightType_Point,
	LightType_Spot,
	LightType_Directional,

	LightType_Max
};

struct Light 
{
	LightType type;
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
protected:
	Light() {}	
};

struct PointLight : Light
{
	PointLight()
	{
		type = LightType_Point;
	}
};

struct SpotLight : Light
{
	SpotLight(const glm::vec3& dir, float angle, float falloff) : 
		direction(dir), angle(angle), falloff(falloff)
	{
		type = LightType_Spot;
	}

	glm::vec3 direction;
	float angle, falloff;
};

struct DirectionalLight : Light
{
	DirectionalLight()
	{
		type = LightType_Directional;
	}	
};

typedef std::shared_ptr<Light> LightPtr;