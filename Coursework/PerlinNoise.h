#pragma once

#include "BaseGenerationMode.h"
#include "vector"

//	Based on the java implementation of Simplex Noise.
/*
	Perlin, K. (2002). Improved Noise Reference Implementation.
	Available: http://mrl.nyu.edu/~perlin/noise/
*/
class PerlinNoise : public BaseGenerationMode
{
public:
	void init();
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
	double noise(double x, double y, double z);
protected:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
protected:
	std::vector<int> permutation;
	std::vector<int> p;
};