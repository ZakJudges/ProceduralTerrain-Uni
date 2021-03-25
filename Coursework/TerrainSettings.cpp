#include "TerrainSettings.h"

TerrainSettings::TerrainSettings()
{
	//	Initialise settings.
	_mode = GENERATIONMODE::FAULT;
	_faultIterations = 10;
	_voronoiRegions = 8;
	_activeRegion = -1;
	_particledepositIterations = 5000;
	_particleDisplacement = 0.15f;
	_noiseFrequency = 0.05f;
	_noiseTime = 0.05f;
	_noiseAmplitude = 10.0f;
	_fbmOctaves = 3;
	_fbmLacunarity = 2.5f;
	_fbmGain = 0.4f;
}


//	For use in displaying the current mode to the user.
char* TerrainSettings::getMode(bool t)
{
	if (_mode == GENERATIONMODE::FAULT)
	{
		return "Faulting";
	}
	else if (_mode == GENERATIONMODE::PERLIN_NOISE)
	{
		return "Simplex Noise";
	}
	else if (_mode == GENERATIONMODE::FBM)
	{
		return "Fractional Brownian Motion";
	}
	else if (_mode == GENERATIONMODE::RANDOM)
	{
		return "Random Noise";
	}
	else if (_mode == GENERATIONMODE::SMOOTH)
	{
		return "Smoothing";
	}
	else if (_mode == GENERATIONMODE::PARTICLE_DEPOSITION)
	{
		return "Particle Deposition";
	}
	else if (_mode == GENERATIONMODE::VORONOI)
	{
		return "Voronoi Regions";
	}
	else if (_mode == GENERATIONMODE::RESET)
	{
		return "Flat Reset";
	}
	else
	{
		return "ERROR";
	}
}

//	Getters.
int TerrainSettings::getVoronoiRegions()
{
	return _voronoiRegions;
}
GENERATIONMODE TerrainSettings::getMode()
{
	return _mode;
}
int TerrainSettings::getActiveRegion()
{
	return _activeRegion;
}
int TerrainSettings::getFaultIterations()
{
	return _faultIterations;
}
int TerrainSettings::getParticleDepositionIterations()
{
	return _particledepositIterations;
}
float TerrainSettings::getParticleDisplacement()
{
	return _particleDisplacement;
}
float TerrainSettings::getNoiseFrequency()
{
	return _noiseFrequency;
}
float TerrainSettings::getNoiseTime()
{
	return _noiseTime;
}
float TerrainSettings::getNoiseAmplitude()
{
	return _noiseAmplitude;
}
int TerrainSettings::getFbmOcataves()
{
	return _fbmOctaves;
}
float TerrainSettings::getFbmLacunarity()
{
	return _fbmLacunarity;
}
float TerrainSettings::getFbmGain()
{
	return _fbmGain;
}

//	Setters.
void TerrainSettings::setFaultIterations(int iterations)
{
	_faultIterations = iterations;
}
void TerrainSettings::setVoronoiRegions(int numberOfRegions)
{
	_voronoiRegions = numberOfRegions;

}
void TerrainSettings::setMode(GENERATIONMODE mode)
{
	_mode = mode;
}
void TerrainSettings::setActiveRegion(int activeRegion)
{
	_activeRegion = activeRegion;
}
void TerrainSettings::setParticleDepositionIterations(int deposits)
{
	_particledepositIterations = deposits;
}
void TerrainSettings::setParticleDisplacement(float displacement)
{
	_particleDisplacement = displacement;
}
void TerrainSettings::setNoiseFrequency(float frequency)
{
	_noiseFrequency = frequency;
}
void TerrainSettings::setNoiseTime(float time)
{
	_noiseTime = time;
}
void TerrainSettings::setNoiseAmplitude(float amplitude)
{
	_noiseAmplitude = amplitude;
}
void TerrainSettings::setFbmOctaves(int octaves)
{
	_fbmOctaves = octaves;
}
void TerrainSettings::setFbmLacunarity(float lacunarity)
{
	_fbmLacunarity = lacunarity;
}
void TerrainSettings::setFbmGain(float gain)
{
	_fbmGain = gain;
}



