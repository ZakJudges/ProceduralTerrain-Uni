// TerrainMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#ifndef _TERRAINMESH_H_
#define _TERRAINMESH_H_

#include "../DXFramework/BaseMesh.h"
#include "SharedData.h"
#include "BaseGenerationMode.h"
#include "Fault.h"
#include "PerlinNoise.h"
#include "TerrainSettings.h"
#include "Voronoi.h"
#include "Smooth.h"
#include "Reset.h"
#include "Particle Deposition.h"
#include "FractalBrownianMotion.h"
#include "RandomNoise.h"

class TerrainMesh : public BaseMesh
{
public:
	TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int terrainWidth, int terrainHeight);
	~TerrainMesh();
	void sendData(ID3D11DeviceContext* deviceContext);
	void initBuffers(ID3D11Device* device);
	void generateHeightMap(ID3D11Device* device, TerrainSettings settings);
	void initialiseHeightMap();
private:
	void calculateNormals();

private:
	int _terrainWidth, _terrainHeight;
	HeightMap* _heightMap;

	BaseGenerationMode* _generationMode;
	PerlinNoise _perlinNoise;
	Fault _fault;
	Voronoi _voronoi;
	Smooth _smooth;
	Reset _reset;
	ParticleDeposition _particleDeposition;
	FractalBrownianMotion _fractalBrownianMotion;
	RandomNoise _randomNoise;
};

#endif