#pragma once

#include "BaseGenerationMode.h"

//	Generate random heights for the terrain.
class RandomNoise : public BaseGenerationMode
{
protected:
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
public:
	void init();
};
