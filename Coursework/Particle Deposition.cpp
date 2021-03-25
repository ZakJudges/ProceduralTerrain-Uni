#include "Particle Deposition.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

void ParticleDeposition::init()
{
	//	Initialise the random seed.
	srand(time(NULL));
}

void ParticleDeposition::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	int index = 0;

	int x = rand() % (terrainWidth - 1);
	int z = rand() % (terrainHeight - 1);

	float displacement = _particleDisplacement;

		for (int i = 0; i < _particleDepositIterations; i++)
		{
			int direction = rand() % 4 + 1;

			//	Use the random direction to set the new point to apply an offset on.
			switch (direction)
			{
				//	Right.
			case 1:
				x++;
				if (x >(terrainWidth - 1))
				{
					x = rand() % (terrainWidth - 1);
				}
				break;
				//	Left.
			case 2:
				x--;
				if (x < 0)
				{
					x = rand() % (terrainWidth - 1);
				}
				break;
				//	Far.
			case 3:
				z++;
				if (z >(terrainHeight - 1))
				{
					z = rand() % (terrainHeight - 1);
				}
				break;
				//	Near.
			case 4:
				z--;
				if (z < 0)
				{
					z = rand() % (terrainHeight - 1);
				}
				break;
			}

			index = (terrainHeight * z) + x;
			if (_heightMap[index].vp == _activeRegion || _activeRegion == -1)
			{
				_heightMap[index].y += displacement;

			}
	}	
}