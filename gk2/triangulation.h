#pragma once
#include <vector>


struct PointData
{
	int x;
	int y;
	double z;
	PointData(int _x, int _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};


class Triangulation
{
	double B_i3(int i, double t);
	double z(double x, double y);
	std::vector<std::vector<PointData>> data;
public:
	Triangulation();
	int divisions_X;
	int divisions_Y;
	void updateTriangulation(int width, int height);
	std::vector<std::vector<PointData>>& getData();
};