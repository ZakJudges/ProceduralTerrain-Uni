#pragma once

#include "PerlinNoise.h"

//	Apply various octaves of Simplex noise.
//	Combining noise:
/*
		Gonzalez, P. Lowe, J. (2015). Fractal Brownian Motion. 
		Available: https://thebookofshaders.com/13/ 
*/
class FractalBrownianMotion : public PerlinNoise
{
public:
	void init(PerlinNoise* perlinNoise);
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
private:
	PerlinNoise* _perlinNoise;
};