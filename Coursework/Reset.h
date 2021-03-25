#pragma once

#include "BaseGenerationMode.h"

//	Set all terrain heights to 0.
class Reset : public BaseGenerationMode
{
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
};