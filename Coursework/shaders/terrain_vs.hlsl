
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


cbuffer VSBuffer : register(b1)
{
    float3 cameraPosition;
    float activeRegion;
    float4 waterSwitch;
    float time;
    float xFrequency;
    float zFrequency;
    float waveHeight;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 additionalData : VORONOI;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 additionalData : VORONOI;
    float activeRegion : ACTIVE_REGION;
    float waterHeight : WATER_HEIGHT;
    float terrainHeight : TERRAIN_HEIGHT;
};

double noise(float x, float y, float z);
double fade(float t);
double lerp(float t, float a, float b);
double grad(int hash, float x, float y, float z);

OutputType main(InputType input)
{
    OutputType output;
    float3 worldPosition;
    
    //  Pass values to pixel shader.
    output.position = input.position;
    output.terrainHeight = input.position.y;
    output.normal = input.normal;
    output.waterHeight = waterSwitch.y;

    //  waterSwitch.x == water enabled, waterSwitch.y = height offset
    if(waterSwitch.x == 1.0f)
    {
        output.position.y = waveHeight * noise(output.position.x * xFrequency, output.position.z * zFrequency, time * 0.3f);
        output.position.y += waterSwitch.y;
        output.waterHeight = output.position.y;
    }

    //  Pass homogeneous clip space coordinate to pixel shader.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    //  Pass Voronoi values to pixel shader.
    output.additionalData = input.additionalData;
    output.activeRegion = activeRegion;

    return output;
}

//  Simplex Noise.
//	Based on the java implementation of Simplex Noise.
/*
	Perlin, K. (2002). Improved Noise Reference Implementation.
	Available: http://mrl.nyu.edu/~perlin/noise/
*/
double noise(float x, float y, float z)
{
    int p[256] = {
        151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};
   
     int X = (int) floor(x) & 255, // FIND UNIT CUBE THAT
		Y = (int) floor(y) & 255, // CONTAINS POINT.
		Z = (int) floor(z) & 255;
    x -= floor(x); // FIND RELATIVE X,Y,Z
    y -= floor(y); // OF POINT IN CUBE.
    z -= floor(z);
    float u = fade(x), // COMPUTE FADE CURVES
		v = fade(y), // FOR EACH OF X,Y,Z.
		w = fade(z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z, // HASH COORDINATES OF
		B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z; // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), // AND ADD
		grad(p[BA], x - 1, y, z)), // BLENDED
		lerp(u, grad(p[AB], x, y - 1, z), // RESULTS
			grad(p[BB], x - 1, y - 1, z))), // FROM  8
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), // CORNERS
			grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

double fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(float t, float a, float b)
{
    return a + t * (b - a);
}

double grad(int hash, float x, float y, float z)
{
    int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
    float u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


