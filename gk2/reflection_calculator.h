#pragma once

#include "glm/vec3.hpp"
#include "imgui.h"
#include "triangulation.h"

class ReflectionCalculator 
{
	static ReflectionCalculator* _instance;
	ReflectionCalculator();
public:
	static ReflectionCalculator* get();
	float kd;
	float ks;
	int m;
	glm::vec3 lightSource;
	ImVec4 objectColor;
	ImVec4 lightColor;
	unsigned calculateColor(glm::vec3 nVector, glm::vec3 pos);
};