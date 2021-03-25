//	Terrain Shader
#include "TerrainShader.h"


TerrainShader::TerrainShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"terrain_vs.cso", L"terrain_ps.cso");
}


TerrainShader::~TerrainShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//release the camera constant buffer
	if (vsBuffer)
	{
		vsBuffer->Release();
		vsBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void TerrainShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC vsBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	//setup the camera buffer
	//setup the description of the camera dynamic constant buffer that is in the vertex shader
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	vsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vsBufferDesc.ByteWidth = sizeof(VSBufferType);
	vsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vsBufferDesc.MiscFlags = 0;
	vsBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&vsBufferDesc, NULL, &vsBuffer);

}




void TerrainShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, 
	const XMMATRIX &projectionMatrix, Light* light, XMFLOAT3 pos, int _activeRegion, bool water, ID3D11ShaderResourceView* rockTexture, 
		ID3D11ShaderResourceView* grassTexture, ID3D11ShaderResourceView* snowTexture, ID3D11ShaderResourceView* waterTexture, WaterSettings waterSettings, bool toonShading)
{
	bool waterMode = water;
	int isVoronoiEnabled;
	int isToonShadingEnabled;

	if (_activeRegion == -1)
	{
		isVoronoiEnabled = 0;
	}
	else
	{
		isVoronoiEnabled = 1;
	}

	if (toonShading)
	{
		isToonShadingEnabled = 1;
	}
	else
	{
		isToonShadingEnabled = 0;
	}

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	VSBufferType* vsPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);



	// Lock the camera constant buffer so it can be written to.
	deviceContext->Map(vsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer
	vsPtr = (VSBufferType*)mappedResource.pData;
	// Copy camera data into the buffer
	vsPtr->cameraPosition = pos;
	vsPtr->activeRegion = _activeRegion;
	if (waterMode)
	{
		vsPtr->waterSwitch = XMFLOAT4(1.0f, waterSettings.heightOffset, 0.0f, 0.0f);
	}
	else
	{
		vsPtr->waterSwitch = XMFLOAT4(0.0f, waterSettings.heightOffset, 0.0f, 0.0f);
	}
	vsPtr->waterSettings = XMFLOAT4(waterSettings.time, waterSettings.xFrequency, waterSettings.yFrequency, waterSettings.height);
	// Unlock the buffer
	deviceContext->Unmap(vsBuffer, 0);
	// Set buffer number (register value)
	bufferNumber = 1;
	// Set the constant buffer in the vertex shader
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &vsBuffer);

	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->ambient = light->getAmbientColour();
	lightPtr->specular = light->getSpecularColour();
	lightPtr->direction = light->getDirection();
	lightPtr->specularPower = light->getSpecularPower();
	if (waterMode)
	{
		lightPtr->settings = XMFLOAT4(1.0f, isVoronoiEnabled, isToonShadingEnabled, 1.0);
	}
	else if (!waterMode)
	{
		lightPtr->settings = XMFLOAT4(0.0f, isVoronoiEnabled, isToonShadingEnabled, 1.0);
	}
	//lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &grassTexture);
	deviceContext->PSSetShaderResources(1, 1, &rockTexture);
	deviceContext->PSSetShaderResources(2, 1, &waterTexture);
	deviceContext->PSSetShaderResources(3, 1, &snowTexture);

}

void TerrainShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



