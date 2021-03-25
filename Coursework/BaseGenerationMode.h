#pragma once

#include "SharedData.h"
#include "TerrainSettings.h"

class BaseGenerationMode
{
public:
	BaseGenerationMode();
	virtual void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);
	void adjustSettings(TerrainSettings settings);

protected:
	//	All terrain generation settings go here.
	//	Each generation mode inherits from this class so keeping the settings as part of the base class... 
	//		...prevents having to reinitialise variables when changing generation modes.
	int _faultIterations;
	int _numberOfRegions;
	int _activeRegion;
	int _particleDepositIterations;
	float _particleDisplacement;
	float _noiseFrequency;
	float _noiseTime;
	float _noiseAmplitude;
	int _fbmOctaves;
	float _fbmLacunarity;
	float _fbmGain;
};