#include "RandomNoise.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

void RandomNoise::init()
{
	srand(time(NULL));
}

void RandomNoise::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	for (int z = 0; z < terrainHeight; z++)
	{
		for (int x = 0; x < terrainWidth; x++)
		{
			int index = (z * terrainHeight) + x;
			if (_heightMap[index].vp == _activeRegion || _activeRegion == -1)
			{
				float offset = (rand() % 5) - 2.5f;
				_heightMap[index].y += offset;
			}
		}
	}
}