#pragma once

#include "SharedData.h"

//	A getter/setter class for terrain settings.
//	Acts as the bridge between ImGui settings and the generation mode settings.
class TerrainSettings
{
public:
	TerrainSettings();

	GENERATIONMODE getMode();
	char* getMode(bool);	//	Boolean so that function can be overloaded.
	int getVoronoiRegions();
	int getFaultIterations();
	int getActiveRegion();
	int getParticleDepositionIterations();
	float getParticleDisplacement();
	float getNoiseFrequency();
	float getNoiseTime();
	float getNoiseAmplitude();
	int getFbmOcataves();
	float getFbmLacunarity();
	float getFbmGain();

	void setVoronoiRegions(int number);
	void setMode(GENERATIONMODE mode);
	void setFaultIterations(int iterations);
	void setActiveRegion(int activeRegion);
	void setParticleDepositionIterations(int deposits);
	void setParticleDisplacement(float displacement);
	void setNoiseFrequency(float frequency);
	void setNoiseTime(float time);
	void setNoiseAmplitude(float amplitude);
	void setFbmOctaves(int octaves);
	void setFbmLacunarity(float lacunarity);
	void setFbmGain(float gain);

	
	
private:
	//	Public variables for use with ImGui.
	int _faultIterations;
	int _particledepositIterations;
	int _voronoiRegions;
	GENERATIONMODE _mode;
	int _activeRegion;
	float _particleDisplacement;
	float _noiseFrequency;
	float _noiseTime;
	float _noiseAmplitude;
	int _fbmOctaves;
	float _fbmLacunarity;
	float _fbmGain;
	


};