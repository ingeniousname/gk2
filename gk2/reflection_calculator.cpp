#include "reflection_calculator.h"
#include "glm/glm.hpp"

ReflectionCalculator* ReflectionCalculator::_instance = nullptr;

ReflectionCalculator::ReflectionCalculator() : lightSource(glm::vec3(1024 / 2, 768 / 2, 100.0))
{
	this->kd = 1;
	this->ks = 0;
	this->m = 100;
	lightColor = ImVec4(1.f, 1.f, 1.f, 1.f);
}

ReflectionCalculator* ReflectionCalculator::get()
{
	if (_instance == nullptr)
	{
		_instance = new ReflectionCalculator();
	}
	return _instance;
}

glm::vec3 ReflectionCalculator::calculateColor(glm::vec3 nVector, glm::vec3 pos)
{
	glm::vec3 L = glm::normalize(lightSource - pos);
	float NLangle = glm::dot(glm::normalize(nVector), L);
	if (NLangle < 0) NLangle = 0;
	glm::vec3 R = 2 * NLangle * nVector - L;
	float VRangle = glm::dot({ 0, 0, 1 }, glm::normalize(R));
	if (VRangle < 0) VRangle = 0;
	VRangle = std::pow(VRangle, m);



	unsigned red = 255 * objectColor.z * lightColor.z * (kd * NLangle + ks * VRangle);
	unsigned green = 255 * objectColor.y * lightColor.y * (kd * NLangle + ks * VRangle);
	unsigned blue = 255 * objectColor.x * lightColor.x * (kd * NLangle + ks * VRangle);
	return { red, green, blue };
}
