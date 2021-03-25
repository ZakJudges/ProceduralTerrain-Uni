#include "BaseGenerationMode.h"

BaseGenerationMode::BaseGenerationMode()
{
	_faultIterations = 1;
	_numberOfRegions = 8;
	_activeRegion = -1;
	_particleDepositIterations = 0;
	_particleDisplacement = 0.0f;
	_noiseFrequency = 0.0f;
	_noiseTime = 0.0f;
	_noiseAmplitude = 0.0f;
	_fbmOctaves = 0;
	_fbmLacunarity = 0.0f;
	_fbmGain = 0.0f;
}

void BaseGenerationMode::adjustSettings(TerrainSettings settings)
{
	_faultIterations = settings.getFaultIterations();
	_numberOfRegions = settings.getVoronoiRegions();
	_activeRegion = settings.getActiveRegion();
	_particleDepositIterations = settings.getParticleDepositionIterations();
	_particleDisplacement = settings.getParticleDisplacement();
	_noiseFrequency = settings.getNoiseFrequency();
	_noiseTime = settings.getNoiseTime();
	_noiseAmplitude = settings.getNoiseAmplitude();
	_fbmOctaves = settings.getFbmOcataves();
	_fbmLacunarity = settings.getFbmLacunarity();
	_fbmGain = settings.getFbmGain();
}

void BaseGenerationMode::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{

}

