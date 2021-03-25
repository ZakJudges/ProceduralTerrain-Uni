#ifndef _SHAREDDATA_H
#define _SHAREDDATA_H

struct HeightMap
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;
	float vp;
};

struct WaterSettings
{
	float time;
	float xFrequency;
	float yFrequency;
	float height;
	float heightOffset;
};

enum GENERATIONMODE {PERLIN_NOISE, FBM, FAULT, PARTICLE_DEPOSITION, RANDOM, SMOOTH, VORONOI, RESET};

#endif