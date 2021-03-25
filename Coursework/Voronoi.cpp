#include "Voronoi.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>


void Voronoi::init(int terrainWidth, int terrainHeight)
{
	srand(time(NULL));

	_terrainWidth = terrainWidth;
	_terrainHeight = terrainHeight;

	_points = new int[_terrainWidth * _terrainHeight];

}

void Voronoi::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	determineRegions(_numberOfRegions);

	for (int z = 0; z < _terrainHeight; z++)
	{
		for (int x = 0; x < _terrainWidth; x++)
		{
			int index = (_terrainHeight * z) + x;

			_heightMap[index].vp = (float)getPoint(index);
		}
	}
}

void Voronoi::determineRegions(int numberOfRegions)
{
	//	Get the random points for the regions.
	for (int i = 0; i < numberOfRegions; i++)
	{
		Point point;
		point.x = rand() % (_terrainWidth - 1);
		point.y = rand() % (_terrainHeight - 1);
		_regions.push_back(point);
	}

	//	Now assign each point in the terrain as a member of a region.
	for (int z = 0; z < _terrainHeight; z++)
	{
		for (int x = 0; x < _terrainWidth; x++)
		{
			int index = (_terrainHeight * z) + x;

			std::vector<float> distances;

			//	Calculate the distance between the current terrain point and each Voronoi Point using Pythagoras.
			for (int i = 0; i < _regions.size(); i++)
			{
				float distance = sqrt(((x - _regions[i].x) * (x - _regions[i].x)) + ((z - _regions[i].y) * (z - _regions[i].y)));
				distances.push_back(distance);
			}
			//	Assign the region number to this point.
			_points[index] = getNearestRegion(distances);
		}
	}
	//	Empty the regions container to prevent unalterable regions.
	_regions.clear();
}

int Voronoi::getNearestRegion(std::vector<float> distances)
{
	//	The region to return.
	int nearestRegion = 0;
	//	The amount of remaining distances to sort.
	int numberLeft = distances.size();
	//	Where to start sorting the distances from.
	int startingPoint = 0;
	//	Counter to detect when all distances have been sorted.
	int count = 0;

	while (numberLeft > 0)
	{
		//	Find the smallest distance.
		for (int i = startingPoint; i < distances.size(); i++)
		{
			if (distances[nearestRegion] > distances[i])
			{
				nearestRegion = i;
				startingPoint = i + 1;
				numberLeft = distances.size() - startingPoint;
				count = 0;
			}
			else
			{
				count += 1;
			}
		}
		if (count >= (distances.size() - startingPoint))
		{
			numberLeft = 0;
		}
	}

	return nearestRegion;
}

int Voronoi::getPoint(int i)
{
	return _points[i];
}

//	Release resources.
Voronoi::~Voronoi()
{
	if (_points)
	{
		delete[] _points;
		_points = 0;
	}
}