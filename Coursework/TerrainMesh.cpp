
#include "TerrainMesh.h"

// Initialise buffer and generation modes.
TerrainMesh::TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int terrainWidth, int terrainHeight)
{
	_terrainWidth = terrainWidth;
	_terrainHeight = terrainHeight;

	initialiseHeightMap();

	_generationMode = new BaseGenerationMode;
	_fault.init();
	_perlinNoise.init();
	_fractalBrownianMotion.init(&_perlinNoise);
	_voronoi.init(terrainWidth, terrainHeight);
	_voronoi.generateHeightMap(_heightMap, _terrainWidth, _terrainHeight);
	_randomNoise.init();

	initBuffers(device);
}

void TerrainMesh::initialiseHeightMap()
{
	_heightMap = new HeightMap[_terrainWidth * _terrainHeight];

	int index = 0;

	// Initialise the data in the height map (flat).
	for (int z = 0; z < _terrainHeight; z++)
	{
		for (int x = 0; x < _terrainWidth; x++)
		{
			index = (_terrainHeight * z) + x;

			_heightMap[index].x = (float)x;
			_heightMap[index].y = 0.0f;
			_heightMap[index].z = (float)z;

			_heightMap[index].nx = 0.0f;
			_heightMap[index].ny = 1.0f;
			_heightMap[index].nz = 0.0f;

			_heightMap[index].tu = 0.0f;
			_heightMap[index].tv = 0.0f;

			_heightMap[index].vp = -1.0f;
			
		}
	}
}

//	Set the data in the heightmap then initialise the vertex and index buffers.
void TerrainMesh::generateHeightMap(ID3D11Device* device, TerrainSettings settings)
{
	if (settings.getMode() == GENERATIONMODE::PERLIN_NOISE)
	{
		_generationMode = &_perlinNoise;
	}
	if (settings.getMode() == GENERATIONMODE::FAULT)
	{
		_generationMode = &_fault;
	}
	if(settings.getMode() == GENERATIONMODE::FBM)
	{
		_generationMode = &_fractalBrownianMotion;
	}
	if (settings.getMode() == GENERATIONMODE::RANDOM)
	{
		_generationMode = &_randomNoise;
	}
	if (settings.getMode() == GENERATIONMODE::SMOOTH)
	{
		_generationMode = &_smooth;
	}
	if (settings.getMode() == GENERATIONMODE::PARTICLE_DEPOSITION)
	{
		_generationMode = &_particleDeposition;
	}
	if (settings.getMode() == GENERATIONMODE::VORONOI)
	{
		_generationMode = &_voronoi;
	}
	if (settings.getMode() == GENERATIONMODE::RESET)
	{
		_generationMode = &_reset;
	}

	_generationMode->adjustSettings(settings);

	_generationMode->generateHeightMap(_heightMap, _terrainWidth, _terrainHeight);

	calculateNormals();

	initBuffers(device);

}

void TerrainMesh::calculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	XMFLOAT3* normals;

	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new XMFLOAT3[(_terrainHeight - 1) * (_terrainWidth - 1)];

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(_terrainHeight - 1); j++)
	{
		for (i = 0; i<(_terrainWidth - 1); i++)
		{
			index1 = (j * _terrainHeight) + i;
			index2 = (j * _terrainHeight) + (i + 1);
			index3 = ((j + 1) * _terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = _heightMap[index1].x;
			vertex1[1] = _heightMap[index1].y;
			vertex1[2] = _heightMap[index1].z;

			vertex2[0] = _heightMap[index2].x;
			vertex2[1] = _heightMap[index2].y;
			vertex2[2] = _heightMap[index2].z;

			vertex3[0] = _heightMap[index3].x;
			vertex3[1] = _heightMap[index3].y;
			vertex3[2] = _heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<_terrainHeight; j++)
	{
		for (i = 0; i<_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (_terrainHeight - 1)))
			{
				index = (j * (_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (_terrainWidth - 1)) && (j < (_terrainHeight - 1)))
			{
				index = (j * (_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * _terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			_heightMap[index].nx = (sum[0] / length);
			_heightMap[index].ny = (sum[1] / length);
			_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

}

// Override sendData() to change topology type. TODO: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ needed for gpu normal calculations
void TerrainMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

// Generate terrian (including texture coordinates, normals and Voronoi points).
void TerrainMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, index1, index2, index3, index4;
	float positionX, positionZ, u, v;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate the number of vertices in the terrain mesh.
	vertexCount = (_terrainWidth - 1) * (_terrainHeight - 1) * 6;
	indexCount = vertexCount;

	//	Create the vertex and index arrays.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	//	Initialise the index to the vertex buffer.
	index = 0;
	float uInc = (1.0f / (float)_terrainWidth) * 4;
	float vInc = (1.0f / (float)_terrainHeight) * 4;
	u = 0.0f;
	v = 0.0f;

	//	Load the vertex and index array with the terrain data.
	for (int z=0; z < (_terrainHeight - 1); z++)
	{
		for (int x = 0; x < (_terrainWidth - 1); x++)
		{
			index1 = (_terrainHeight * z) + x;          // Bottom left.
			index2 = (_terrainHeight * z) + (x + 1);      // Bottom right.
			index3 = (_terrainHeight * (z + 1)) + x;      // Upper left.
			index4 = (_terrainHeight * (z + 1)) + (x + 1);  // Upper right.

			// Upper left.
			vertices[index].position = XMFLOAT3(_heightMap[index3].x, _heightMap[index3].y, _heightMap[index3].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index3].nx, _heightMap[index3].ny, _heightMap[index3].nz);
			vertices[index].texture = XMFLOAT2(u, v + vInc);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index3].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;


			// Upper right.
			vertices[index].position = XMFLOAT3(_heightMap[index4].x, _heightMap[index4].y, _heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index4].nx, _heightMap[index4].ny, _heightMap[index4].nz);
			vertices[index].texture = XMFLOAT2(u + uInc, v + vInc);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index4].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_heightMap[index1].x, _heightMap[index1].y, _heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index1].nx, _heightMap[index1].ny, _heightMap[index1].nz);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index1].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_heightMap[index1].x, _heightMap[index1].y, _heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index1].nx, _heightMap[index1].ny, _heightMap[index1].nz);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index1].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(_heightMap[index4].x, _heightMap[index4].y, _heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index4].nx, _heightMap[index4].ny, _heightMap[index4].nz);
			vertices[index].texture = XMFLOAT2(u + uInc, v + vInc);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index4].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(_heightMap[index2].x, _heightMap[index2].y, _heightMap[index2].z);
			vertices[index].normal = XMFLOAT3(_heightMap[index2].nx, _heightMap[index2].ny, _heightMap[index2].nz);
			vertices[index].texture = XMFLOAT2(u + uInc, v);
			vertices[index].voronoiPoint = XMFLOAT4(_heightMap[index2].vp, 0.0f, 0.0f, 0.0f);
			indices[index] = index;
			index++;

			u += uInc;
		}
		u = 0.0f;
		v += vInc;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

// Release resources.
TerrainMesh::~TerrainMesh()
{
	if (_heightMap)
	{
		delete[] _heightMap;
		_heightMap = 0;
	}

	if (_generationMode)
	{
		delete _generationMode;
		_generationMode = 0;
	}

	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

