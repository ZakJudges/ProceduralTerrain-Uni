#include "FractalBrownianMotion.h"

void FractalBrownianMotion::init(PerlinNoise* perlinNoise)
{
	_perlinNoise = perlinNoise;
}

void FractalBrownianMotion::generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight)
{
	double frequency = _noiseFrequency;
	float amplitude = _noiseAmplitude;
	float offset = 0.0f;

	//	amplitude - range of values, frequency - the period at which the data is sampled.
	//	lacunarity - scale for the frequency per octave, gain - scale for the amplitude per octave.
		for (int z = 0; z < terrainHeight; z++)
		{
			for (int x = 0; x < terrainWidth; x++)
			{
				int index = (terrainHeight * z) + x;

				if (_heightMap[index].vp == _activeRegion || _activeRegion == -1)
				{
					//	Loop through the number of octaves, applying noise with different variables each time.
					for (int i = 0; i < _fbmOctaves; i++)
					{
						offset += (amplitude * _perlinNoise->noise(x * frequency, z * frequency, _noiseTime));
						frequency *= _fbmLacunarity;
						amplitude *= _fbmGain;
					}

					//	Apply the height to the vertex then reset the variables.
					_heightMap[index].y = offset;
					offset = 0.0f;
					frequency = _noiseFrequency;
					amplitude = _noiseAmplitude;

				}
			}
		}
	
}
