// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "TerrainShader.h"
#include "ToonShader.h"
#include "TerrainMesh.h"
#include "SharedData.h"
#include "TerrainSettings.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void RenderScene();
	void ToonShading();
	void GenerateTerrain();
	void gui();

private:
	// For normal scene rendering.
	TerrainShader* terrainShader;
	TerrainMesh* terrainMesh;
	Light* light;

	PlaneMesh* waterMesh;
	bool renderWater;

	// For rendering the scene to a texture for toon shading.
	RenderTexture* renderTexture;
	ToonShader* toonShader;
	OrthoMesh* orthoMesh;

	//	ImGui
	bool isWireframe;
	bool generate;
	TerrainSettings settings;
	WaterSettings waterSettings;

	bool toonShading;
	
};

#endif