#include "triangulation.h"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "SDL_image.h"

double Triangulation::B_i3(int i, double t)
{
	const int fact[] = { 1, 1, 2, 6 };
	double res = 6 / (fact[i] * fact[3 - i]);
	for (int k = 0; k < i; k++)
		res *= t;
	for (int k = 0; k < 3 - i; k++)
		res *= (1 - t);
	return res;
}

double Triangulation::z(double x, double y)
{
	double res = 0.0;
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			res += z_point[i][j] * B_i3(i, x) * B_i3(j, y);

	return res;
}



Triangulation::Triangulation()
{
	divisions_X = 30;
	divisions_Y = 30;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			z_point[i][j] = 0;
		}
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

			const double h = 1e-8;
			double dzx = (z((double)x / width + h, (double)y / height) - z((double)x / width - h, (double)y / height)) / (2 * h);
			double dzy = (z((double)x / width, (double)y / height + h) - z((double)x / width, (double)y / height - h)) / (2 * h);
			glm::vec3 nVector = glm::normalize(glm::cross(glm::vec3{ 1, 0, dzx }, glm::vec3{ 0, 1, dzy }));
			PointData p(
				x,
				y,
				z((double)x / width, (double)y / height),
				nVector);
			innerData.push_back(p);
		}
		data.push_back(innerData);
	}
	return;
}

void Triangulation::drawTriangluation(SDL_Renderer* r)
{
	SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
	for (int i = 0; i <= divisions_X; i++)
	{
		SDL_RenderDrawLine(r, data[i][0].x, data[i][0].y, data[i][divisions_Y].x, data[i][divisions_Y].y);

		for (int j = 0; j <= divisions_Y; j++)
		{
			if (i < divisions_X && j > 0)
			{
				SDL_RenderDrawLine(r, data[i][j].x, data[i][j].y, data[i + 1][j - 1].x, data[i + 1][j - 1].y);
			}
		}
	}
	for (int j = 0; j <= divisions_Y; j++)
		SDL_RenderDrawLine(r, data[0][j].x, data[0][j].y, data[divisions_X][j].x, data[divisions_X][j].y);
}


std::vector<std::vector<PointData>>& Triangulation::getData()
{
	return this->data;
}
