#pragma once

#include "glm/vec3.hpp"
#include "imgui.h"
#include "triangulation.h"
#include "SDL.h"
#include <string>

class ReflectionCalculator 
{
	static ReflectionCalculator* _instance;
	ReflectionCalculator();
	SDL_Surface* objectTexture = NULL;
	SDL_Surface* normalMap = NULL;
public:
	static ReflectionCalculator* get();
	float kd;
	float ks;
	int m;
	glm::vec3 lightSource;
	ImVec4 objectColor;
	ImVec4 lightColor;
	std::string objectTexturePath = "";
	std::string normalMapPath = "";
	bool hasNormalMap();
	bool loadNormalMap();
	void deleteNormalMap();
	bool loadObjectTexture();
	void deleteObjectTexture();
	bool hasTexture();
	unsigned calculateColor(glm::vec3 nVector, glm::vec3 pos);
};