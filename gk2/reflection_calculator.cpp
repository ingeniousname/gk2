#include "reflection_calculator.h"
#include "glm/glm.hpp"
#include "SDL_image.h"
#include "glm/gtx/norm.hpp"


ReflectionCalculator* ReflectionCalculator::_instance = nullptr;

ReflectionCalculator::ReflectionCalculator() : lightSource(glm::vec3(1024 / 2, 768 / 2, 100.0))
{
	this->kd = 1;
	this->ks = 0;
	this->m = 100;
	lightColor = ImVec4(1.f, 1.f, 1.f, 1.f);
	objectTexturePath = "sample.jpg";
}

ReflectionCalculator* ReflectionCalculator::get()
{
	if (_instance == nullptr)
	{
		_instance = new ReflectionCalculator();
	}
	return _instance;
}

unsigned ReflectionCalculator::calculateColor(glm::vec3 nVector, glm::vec3 pos)
{
	glm::vec3 L = glm::normalize(lightSource - pos);

	if (this->hasNormalMap())
	{
		glm::vec3 B = glm::normalize(glm::cross(nVector, glm::vec3(0, 0, 1)));
		if (glm::l1Norm(glm::vec3(0, 0, 1), nVector) < 1e-6)
			B = glm::vec3(0, 1, 0);
		glm::vec3 T = glm::normalize(glm::cross(B, nVector));
		glm::mat3x3 transformNormalMatrix(T, B, nVector);

		Uint32* data = (Uint32*)((Uint8*)normalMap->pixels + (normalMap->format->BytesPerPixel * (((int)pos.y % normalMap->h) * normalMap->w + ((int)pos.x % normalMap->w))));
		//glm::vec3 NTexture = glm::vec3(
		//	(*data & normalMap->format->Rmask) >> normalMap->format->Rshift,
		//	(*data & normalMap->format->Gmask) >> normalMap->format->Gshift,
		//	(*data & normalMap->format->Bmask) >> normalMap->format->Bshift);


		nVector = glm::normalize(transformNormalMatrix * glm::vec3(
			((float)((*data & normalMap->format->Rmask) >> normalMap->format->Rshift) - 127.f) / 128.f,
			((float)((*data & normalMap->format->Gmask) >> normalMap->format->Gshift) - 127.f) / 128.f,
			(float)((*data & normalMap->format->Bmask) >> normalMap->format->Bshift) / 255.f));
	}

	float NLangleCos = glm::dot(glm::normalize(nVector), L);
	if (NLangleCos < 0) NLangleCos = 0;
	glm::vec3 R = 2 * NLangleCos * nVector - L;
	float VRangleCos = glm::dot({ 0, 0, 1 }, glm::normalize(R));
	if (VRangleCos < 0) VRangleCos = 0;
	VRangleCos = std::pow(VRangleCos, m);

	Uint8 red = 2, green, blue;
	if (objectTexture == NULL)
	{
		red = 255 * objectColor.x * lightColor.z * (kd * NLangleCos + ks * VRangleCos);
		green = 255 * objectColor.y * lightColor.y * (kd * NLangleCos + ks * VRangleCos);
		blue = 255 * objectColor.z * lightColor.x * (kd * NLangleCos + ks * VRangleCos);
	}
	else
	{
		
		Uint32* data = (Uint32*)((Uint8*)objectTexture->pixels + (int)(objectTexture->format->BytesPerPixel * (((int)pos.y % objectTexture->h) * objectTexture->w + ((int)pos.x % objectTexture->w))));
		red = std::min((float)((*data & objectTexture->format->Rmask) >> objectTexture->format->Rshift) * lightColor.z * (kd * NLangleCos + ks * VRangleCos), 255.f);
		green = std::min((float)((*data & objectTexture->format->Gmask) >> objectTexture->format->Gshift) * lightColor.y * (kd * NLangleCos + ks * VRangleCos), 255.f);
		blue = std::min((float)((*data & objectTexture->format->Bmask) >> objectTexture->format->Bshift) * lightColor.x * (kd * NLangleCos + ks * VRangleCos), 255.f);
	}
	return (255 << 24) | (red << 16) | (green << 8) | blue;
}

bool ReflectionCalculator::loadObjectTexture()
{
	deleteObjectTexture();
	objectTexture = IMG_Load(objectTexturePath.c_str());
	if (objectTexture != NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

bool ReflectionCalculator::hasTexture()
{
	return objectTexture != NULL;
}



void ReflectionCalculator::deleteObjectTexture()
{
	if (objectTexture != NULL)
	{
		SDL_FreeSurface(objectTexture);
		objectTexture = NULL;
	}

}

bool ReflectionCalculator::hasNormalMap()
{
	return normalMap != NULL;
}

bool ReflectionCalculator::loadNormalMap()
{
	deleteNormalMap();
	normalMap = IMG_Load(normalMapPath.c_str());
	if (normalMap != NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void ReflectionCalculator::deleteNormalMap()
{
	if (normalMap != NULL)
	{
		SDL_FreeSurface(normalMap);
		normalMap = NULL;
	}
}
