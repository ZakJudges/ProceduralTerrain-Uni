#include "Smooth.h"

void Smooth::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	//	Loop throught each terrain point, if in the active Voronoi region then find the average of the surrounding neightbours heights.
	for (int z = 0; z < terrainHeight; z++)
	{
		for (int x = 0; x < terrainWidth; x++)
		{
			if (_heightMap[terrainHeight * z + x].vp == _activeRegion || _activeRegion == -1)
			{
				//	Terrain Coordinates.
				int centre = (terrainHeight * z) + x;
				int right = (terrainHeight * z) + (x + 1);
				int down = (terrainHeight * (z + 1)) + x;
				int left = (terrainHeight * z) + (x - 1);
				int up = (terrainHeight * (z - 1)) + x;

				//	If a coordinate is out of bounds then make it the current point.
				if (z == 0)
				{
					up = centre;
				}
				if (x == 0)
				{
					left = centre;
				}
				if (z == (terrainHeight - 1))
				{
					down = centre;
				}
				if (x == (terrainWidth - 1))
				{
					right = centre;
				}

				//	Find the average height.
				_heightMap[centre].y = (_heightMap[right].y + _heightMap[down].y + _heightMap[left].y + _heightMap[up].y) + (_heightMap[centre].y);
				_heightMap[centre].y /= 5;
			}
		}
	}
}