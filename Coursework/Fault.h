#pragma once

#include "BaseGenerationMode.h"

//	Generate fault lines on the terrain.
//	Based on method described by Lighthouse3D.
/* 
	Lighthouse3d. (no date). The Fault Algorithm.
	Available: http://www.lighthouse3d.com/opengl/terrain/index.php?fault 
*/
class Fault : public BaseGenerationMode
{
public:
	void init();
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
};