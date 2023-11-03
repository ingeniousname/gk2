#include "triangle_filler.h"
#include "triangulation.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <thread>

double TriangleFiller::getZ(std::vector<PointData> data, int width, int height)
{
	double x = 0.0, y = 0.0;
	for (int i = 0; i < data.size(); i++)
	{
		x += data[i].x;
		y += data[i].y;
	}
	x /= data.size();
	y /= data.size();
	return (width - std::abs(2 * x - width)) / width * 127.0 + (height - std::abs(2 * y - height)) / height * 127.0;
}

double TriangleFiller::getM(const PointData& p1, const PointData& p2)
{
	return p1.x == p2.x ? std::numeric_limits<double>::infinity() : (double)(p2.y - p1.y) / (p2.x - p1.x);
}

void TriangleFiller::fillPolygon(Uint8* pixels, std::vector<PointData> data, int width, int height)
{
	std::vector<int> indicies = std::vector<int>(data.size());
	double z = 0;
	for (int i = 0; i < data.size(); i++)
	{
		indicies[i] = i;
		z += data[i].z;
	}
	z /= data.size();
	std::sort(indicies.begin(), indicies.end(), [&data](int p1, int p2) { return data[p1].y < data[p2].y; });


	int ymin = data[indicies[0]].y;
	int ymax = data[indicies[indicies.size() - 1]].y;

	std::vector<ActiveEdgeTableNode> AET;
	int AETindex = 0;
	bool firstIter = true;
	
	for (int y = ymin; y <= ymax; y++)
	{
		for (int idx = 0; idx < AET.size(); idx = idx + 2)
		{
			for (int x = AET[idx].x; x <= AET[idx + 1].x; x++)
			{
				Uint8* ptr = pixels + (4 * (y * width + x));
				ptr[0] = z / 3888 * 255;
				ptr[1] = z / 3888 * 255;
				ptr[2] = z / 3888 * 255;
				ptr[3] = 255;
			}
			AET[idx].x += AET[idx].one_m;
			AET[idx + 1].x += AET[idx + 1].one_m;
		}

		AET.erase(std::remove_if(AET.begin(), AET.end(), [y](ActiveEdgeTableNode n1) {return n1.ymax == y; }), AET.end());
		while (AETindex < indicies.size() && data[indicies[AETindex]].y == y)
		{

			int previdx = indicies[AETindex] - 1 < 0 ? indicies.size() - 1 : indicies[AETindex] - 1;
			int nextidx = (indicies[AETindex] + 1) % indicies.size();

			if (data[nextidx].y >= y)
			{
				double m = getM(data[nextidx], data[indicies[AETindex]]);
				if(m != 0)
					AET.push_back(ActiveEdgeTableNode(data[nextidx].y, data[indicies[AETindex]].x, 1 / m));
			}

			if (data[previdx].y >= y)
			{
				double m = getM(data[previdx], data[indicies[AETindex]]);
				if(m != 0)
					AET.push_back(ActiveEdgeTableNode(data[previdx].y, data[indicies[AETindex]].x, 1 / m));
			}
			
			AETindex++;
		}

		std::sort(AET.begin(), AET.end(), [](ActiveEdgeTableNode n1, ActiveEdgeTableNode n2) { return n1.x < n2.x; });
		if (firstIter)
		{
			y--;
			firstIter = false;
		}
	}

}

void TriangleFiller::fillRectangularTriangleAngleTop(Uint8* pixels, std::vector<PointData> data, int width, int height)
{
	int startx = data[1].x;
	double endx = data[0].x;
	double one_m = 1 / getM(data[0], data[2]);
	int ymin = data[0].y;
	int ymax = data[2].y;
	//double z = getZ(data, width, height);
	for (int y = ymin; y <= ymax; y++)
	{
		for (int x = startx; x <= endx; x++)
		{
			Uint8* ptr = pixels + (4 * (y * width + x));
			ptr[0] = 255;
			ptr[1] = 255;
			ptr[2] = 255;
			ptr[3] = 255;
		}
		endx += one_m;
	}

}

void TriangleFiller::fillRectangularTriangleAngleBottom(Uint8* pixels, std::vector<PointData> data, int width, int height)
{
	double startx = data[2].x;
	int endx = data[1].x;
	double one_m = 1 / getM(data[0], data[2]);
	int ymin = data[2].y;
	int ymax = data[0].y;
	//double z = getZ(data, width, height);

	for (int y = ymin; y <= ymax; y++)
	{
		for (int x = startx; x <= endx; x++)
		{
			Uint8* ptr = pixels + (4 * (y * width + x));
			ptr[0] = 255;
			ptr[1] = 0;
			ptr[2] = 0;
			ptr[3] = 255;
		}
		startx += one_m;
	}
}

void TriangleFiller::fillTriangles(Uint8* pixels, std::vector<std::vector<PointData>> pointData, int width, int height)
{
	for (int i = 0; i < pointData.size() - 1; i++)
	{
		for (int j = 0; j < pointData[i].size() - 1; j++)
		{
			std::vector<PointData> t1, t2;

			t1.push_back(pointData[i + 1][j]);
			t1.push_back(pointData[i][j]);
			t1.push_back(pointData[i][j + 1]);

			t2.push_back(pointData[i][j + 1]);
			t2.push_back(pointData[i + 1][j + 1]);
			t2.push_back(pointData[i + 1][j]);

			//fillPolygon(pixels, t1, width, height);
			//fillPolygon(pixels, t2, width, height);


			fillRectangularTriangleAngleTop(pixels, t1, width, height);
			fillRectangularTriangleAngleBottom(pixels, t2, width, height);
		}
	}

}

bool AETComparator::operator()(const ActiveEdgeTableNode& n1, const ActiveEdgeTableNode& n2)
{
	return n1.x < n2.x;
}
