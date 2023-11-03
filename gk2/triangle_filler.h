#pragma once
#include "SDL.h"
#include <vector>

struct PointData;

struct ActiveEdgeTableNode 
{
	int ymax;
	double x;
	double one_m;
	ActiveEdgeTableNode(int _ymax, double _x, double _one_m)
	{
		ymax = _ymax;
		x = _x;
		one_m = _one_m;
	}
};

class AETComparator
{
public:
	bool operator()(const ActiveEdgeTableNode& n1, const ActiveEdgeTableNode& n2);
};

class TriangleFiller
{
	static double getZ(std::vector<PointData> data, int width, int height);
	static double getM(const PointData& p1, const PointData& p2);
	static void fillRectangularTriangleAngleTop(Uint8* pixels, std::vector<PointData> data, int width, int height);
	static void fillRectangularTriangleAngleBottom(Uint8* pixels, std::vector<PointData> data, int width, int height);
public:
	static void fillPolygon(Uint8* pixels, std::vector<PointData> data, int width, int height);
	static void fillTriangles(Uint8* pixels, std::vector<std::vector<PointData>> pointData, int width, int height);
};