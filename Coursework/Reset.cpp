#include "Reset.h"

void Reset::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	for (int z = 0; z < terrainHeight; z++)
	{
		for (int x = 0; x < terrainWidth; x++)
		{
			int index = (terrainHeight * z) + x;

			if (_heightMap[index].vp == _activeRegion || _activeRegion == -1)
			{
				_heightMap[index].y = 0.0f;

			}
		}
	}
	
}