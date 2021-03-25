#pragma once

#include "BaseGenerationMode.h"

//	Blur the terrain vertices together to increase smoothness.
class Smooth : public BaseGenerationMode
{
public:
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);

};