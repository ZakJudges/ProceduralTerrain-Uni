#pragma once

#include "BaseGenerationMode.h"

//	Deposit particles on the terrain.
//	Based on method described by Lighthouse3D.
/*
	Lighthouse3d. (no date). Particle Deposition. 
	Available: http://www.lighthouse3d.com/opengl/terrain/index.php?particle
*/
class ParticleDeposition : public BaseGenerationMode
{
public:
	void init();
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);

};