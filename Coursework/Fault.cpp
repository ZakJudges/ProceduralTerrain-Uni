#include "Fault.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

void Fault::init()
{
	//	Initialise the random seed so that different points are chosen each time a new fault object is created.
	srand(time(NULL));
}

void Fault::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	float x1, x2, z1, z2;
	float displacement = 0.6f;
	

	for (int i = 0; i < _faultIterations; i++)
	{
		if (displacement < 0.05f)
		{
			displacement = 0.05f;
		}

		//	Get random points for a vector
		x1 = rand() % (terrainWidth - 1);
		x2 = rand() % (terrainWidth - 1);
		z1 = rand() % (terrainHeight - 1);
		z2 = rand() % (terrainHeight - 1);

		//	Loop through each point in the terrain and determine which side of the vector the point is on.
		for (int z = 0; z < terrainHeight; z++)
		{
			for (int x = 0; x < terrainWidth; x++)
			{
				int index = (terrainHeight * z) + x;
				
				if (_heightMap[index].vp == _activeRegion || _activeRegion == -1)
				{
					float crossProductResult = (x2 - x1) * (_heightMap[index].z - z1) - (z2 - z1)*(_heightMap[index].x - x1);

					if (crossProductResult > 0.0f)
					{
						_heightMap[index].y += displacement;
					}
					else
					{
						_heightMap[index].y -= displacement;
					}
				}
			}
		}
		displacement -= 0.03f;
	}
}