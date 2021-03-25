// Terrain Shader.h
#ifndef _TERRAINSHADER_H_
#define _TERRAINSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "SharedData.h"

using namespace std;
using namespace DirectX;


class TerrainShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
		XMFLOAT3 direction;
		float specularPower;
		XMFLOAT4 settings;	//x = water enabled, y = voronoi enabled, z = active region.
	};

	struct VSBufferType
	{
		XMFLOAT3 cameraPosition;
		float activeRegion;
		XMFLOAT4 waterSwitch;
		XMFLOAT4 waterSettings;
	};

public:

	TerrainShader(ID3D11Device* device, HWND hwnd);
	~TerrainShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		Light* light, XMFLOAT3 pos, int activeRegion, bool water, ID3D11ShaderResourceView* rockTexture, ID3D11ShaderResourceView* grassTexture, 
		ID3D11ShaderResourceView* snowTexture, ID3D11ShaderResourceView* waterTexture, WaterSettings waterSettings, bool toonShading);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);


private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* vsBuffer;
};

#endif