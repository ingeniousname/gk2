#pragma once
#include <vector>
#include <array>
#include <string>
#include "glm/vec3.hpp"
#include "SDL.h"


struct PointData
{
	int x;
	int y;
	double z;
	glm::vec3 normal;
	PointData(int _x, int _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
		normal = { 0.f, 0.f, 1.f };
	}

	PointData(int _x, int _y, double _z, glm::vec3 n)
	{
		x = _x;
		y = _y;
		z = _z;
		normal = n;
	}
};


class Triangulation
{
	double B_i3(int i, double t);
	double z(double x, double y);
	std::vector<std::vector<PointData>> data;
	SDL_Surface* normalMap = NULL;
public:
	std::string normalMapPath = "";
	std::array<std::array<float, 4>, 4> z_point;
	Triangulation();
	int divisions_X;
	int divisions_Y;
	void updateTriangulation(int width, int height);
	bool hasNormalMap();
	bool loadNormalMap(SDL_Renderer* r);
	void deleteNormalMap();
	std::vector<std::vector<PointData>>& getData();
};