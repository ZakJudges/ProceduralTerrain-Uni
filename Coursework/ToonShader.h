// texture shader.h
#ifndef _TOONSHADER_H_
#define _TOONSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class ToonShader : public BaseShader
{

public:

	struct ScreenSizeBufferType
	{
		float screenWidth;
		float screenHeight;
		XMFLOAT2 padding;
	};

	ToonShader(ID3D11Device* device, HWND hwnd);
	~ToonShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view,
		const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float screenWidth, float screenHeight);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;

};

#endif