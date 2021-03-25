// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	terrainMesh = nullptr;
	terrainShader = nullptr;
	waterMesh = nullptr;
	renderTexture = nullptr;
	orthoMesh = nullptr;
	toonShader = nullptr;
	light = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("water", L"../res/WaterTexture.jpg");
	textureMgr->loadTexture("rock", L"../res/RockTexture.jpg");
	textureMgr->loadTexture("grass", L"../res/GrassTexture2.jpg");
	textureMgr->loadTexture("snow", L"../res/snow.jpg");

	// Create the terrain mesh and shader objects.
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 99, 99);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);

	//	Create the water mesh and shader objects.
	waterMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	// Render texture, OrthoMesh and shader set for different render target.
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0, 0);
	toonShader = new ToonShader(renderer->getDevice(), hwnd);

	light = new Light;
	light->setDirection(0.0f, -1.0f, 0.0f);
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setSpecularPower(50.0f);

	renderWater = false;

	//	Initialise water settings variables.
	waterSettings.time = 0.0f;
	waterSettings.height = 1.3f;
	waterSettings.xFrequency = 0.21f;
	waterSettings.yFrequency = 0.09f;
	waterSettings.heightOffset = -5.0f;

	toonShading = false;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (terrainMesh)
	{
		delete terrainMesh;
		terrainMesh = 0;
	}
	if (waterMesh)
	{
		delete waterMesh;
		waterMesh = 0;
	}
	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = 0;
	}
	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}
	if (orthoMesh)
	{
		delete orthoMesh;
		orthoMesh = 0;
	}
	if (toonShader)
	{
		delete toonShader;
		toonShader = 0;
	}
	if (light)
	{
		delete light;
		light = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	if (generate)
	{
		GenerateTerrain();
		generate = false;
	}

	waterSettings.time += timer->getTime();

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

void App1::GenerateTerrain()
{
	terrainMesh->generateHeightMap(renderer->getDevice(), settings);
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	renderer->setWireframeMode(isWireframe);

	RenderScene();

	if (toonShading)
	{
		ToonShading();

	}
	
	// Render GUI
	gui();

	//	Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	if (toonShading)
	{
		//	Set the render target to be the render to texture.
		renderTexture->setRenderTarget(renderer->getDeviceContext());
		renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);
	}
	else if (!toonShading)
	{
		renderer->setBackBufferRenderTarget();
	}

	//	Generate the view matrix based on the camera's position.
	camera->update();

	//	Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	worldMatrix = XMMatrixTranslation(-50.0f, -4.0f, 10.0f);
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//	Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderWater = false;
	terrainMesh->sendData(renderer->getDeviceContext());
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		light, camera->getPosition(), settings.getActiveRegion(), renderWater, textureMgr->getTexture("rock"), textureMgr->getTexture("grass"),
			textureMgr->getTexture("snow"), textureMgr->getTexture("water"), waterSettings, toonShading);
	// Render the object (combination of mesh geometry and shader process).
	terrainShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	renderWater = true;

	waterMesh->sendData(renderer->getDeviceContext());
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		light, camera->getPosition(), settings.getActiveRegion(), renderWater, textureMgr->getTexture("rock"), textureMgr->getTexture("grass"),
		textureMgr->getTexture("snow"), textureMgr->getTexture("water"), waterSettings, toonShading);
	terrainShader->render(renderer->getDeviceContext(), waterMesh->getIndexCount());

}


void App1::ToonShading()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	//	Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects. 
	worldMatrix = renderer->getWorldMatrix();

	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//	To render ortho mesh.
	//	Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	//	Ortho matrix for 2D rendering.
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());
	toonShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, 
		renderTexture->getShaderResourceView(), sWidth, sHeight);
	toonShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);
	
}


void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	//	Make copies of the setings variables so they can be altered by ImGui.
	int mode = settings.getMode();
	int faultIterations = settings.getFaultIterations();
	int numberOfRegions = settings.getVoronoiRegions();
	int activeRegion = settings.getActiveRegion();
	int particleDepositionIterations = settings.getParticleDepositionIterations();
	float particleDisplacement = settings.getParticleDisplacement();
	float noiseFrequency = settings.getNoiseFrequency();
	float noiseTime = settings.getNoiseTime();
	float noiseAmplitude = settings.getNoiseAmplitude();
	int fbmOctaves = settings.getFbmOcataves();
	float fbmLacunarity = settings.getFbmLacunarity();
	float fbmGain = settings.getFbmGain();


	// Build UI
	ImGui::Checkbox("Generate", &generate);
	ImGui::SameLine();
	ImGui::Text(settings.getMode(true));
	ImGui::SliderInt("Generation Mode", &mode, 0, 7);
	ImGui::Separator();
	ImGui::SliderInt("Active Region", &activeRegion, -1, settings.getVoronoiRegions() - 1);
	ImGui::Separator();
	if (ImGui::BeginMenu("Terrain Settings"))
	{
		ImGui::SliderInt("Fault Iterations", &faultIterations, 0, 50);
		ImGui::SliderInt("Number of Voronoi Regions", &numberOfRegions, 1, 100);
		ImGui::SliderInt("Particle Deposition Iterations", &particleDepositionIterations, 100, 10000);
		ImGui::SliderFloat("Particle Deposition Displacement", &particleDisplacement, -3.0f, 3.0f);
		ImGui::SliderFloat("Noise Frequency", &noiseFrequency, -1.0f, 1.0f);
		ImGui::SliderFloat("Noise Time", &noiseTime, 0.0f, 100.0f);
		ImGui::SliderFloat("Noise Amplitude", &noiseAmplitude, -25.0f, 25.0f);
		ImGui::SliderInt("FBM Octaves", &fbmOctaves, 1, 20);
		ImGui::SliderFloat("FBM Lacunarity", &fbmLacunarity, 0.1f, 5.0f);
		ImGui::SliderFloat("FBM Gain", &fbmGain, 0.1f, 2.0f);
		ImGui::EndMenu();
	}
	ImGui::Separator();
	if (ImGui::BeginMenu("Water Settings"))
	{
		ImGui::SliderFloat("X Frequency ", &waterSettings.xFrequency, 0.01f, 3.01f);
		ImGui::SliderFloat("Z Frequency ", &waterSettings.yFrequency, 0.01f, 3.01f);
		ImGui::SliderFloat("Wave Height" , &waterSettings.height, 0.01f, 5.0f);
		ImGui::SliderFloat("Position ", &waterSettings.heightOffset, -30.0f, 20.0f);
		ImGui::Text("Time: %f", waterSettings.time);
		ImGui::EndMenu();
	}
	ImGui::Separator();
	if (ImGui::BeginMenu("Post Processing Settings"))
	{
		ImGui::Checkbox("Toon Shading", &toonShading);
		ImGui::EndMenu();
	}
	

	ImGui::Separator();
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::SameLine();
	ImGui::Checkbox("Wireframe", &isWireframe);

	//	Pass the updated settings variables back to the settings objects.
	settings.setMode((GENERATIONMODE)mode);
	settings.setFaultIterations(faultIterations);
	settings.setVoronoiRegions(numberOfRegions);
	settings.setActiveRegion(activeRegion);
	settings.setParticleDepositionIterations(particleDepositionIterations);
	settings.setParticleDisplacement(particleDisplacement);
	settings.setNoiseFrequency(noiseFrequency);
	settings.setNoiseTime(noiseTime);
	settings.setNoiseAmplitude(noiseAmplitude);
	settings.setFbmOctaves(fbmOctaves);
	settings.setFbmLacunarity(fbmLacunarity);
	settings.setFbmGain(fbmGain);
	

	// Render UI
	ImGui::Render();
}
