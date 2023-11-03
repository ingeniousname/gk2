#include "triangulation.h"

double Triangulation::B_i3(int i, double t)
{
	const int fact[] = { 1, 1, 2, 6 };
	double res = 6 / fact[i] * fact[3 - i];
	for (int k = 0; k < i; k++)
		res *= t;
	for (int k = 0; k < 3 - i; k++)
		res *= (1 - t);
	return res;
}

double Triangulation::z(double x, double y)
{
	double res = 0.0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res += B_i3(i, x) * B_i3(j, y);

	return res;
}

Triangulation::Triangulation()
{
	divisions_X = 30;
	divisions_Y = 30;
}

void Triangulation::updateTriangulation(int width, int height)
{
	data.clear();
	int xoffset = width / divisions_X;
	int yoffset = height / divisions_Y;

	int xoverflow = width % divisions_X;
	int yoverflow = height % divisions_Y;

	int global_x_offset = 0;

	for (int i = 0; i <= divisions_X; i++)
	{
		if (xoverflow > 0)
		{
			global_x_offset++;
			xoverflow--;
		}
		int local_yoverflow = yoverflow;
		int local_y_offset = 0;



		std::vector<PointData> innerData;
		for (int j = 0; j <= divisions_Y; j++)
		{
			if (local_yoverflow > 0)
			{
				local_y_offset++;
				local_yoverflow--;
			}
			int x = i == divisions_X ? width - 1 : xoffset * i + global_x_offset;
			int y = j == divisions_Y ? height - 1 : yoffset * j + local_y_offset;
			PointData p(
				x,
				y,
				z((double)x / width, (double)y / height)
			);
			innerData.push_back(p);
		}
		data.push_back(innerData);
	}

}

std::vector<std::vector<PointData>>& Triangulation::getData()
{
	return this->data;
}