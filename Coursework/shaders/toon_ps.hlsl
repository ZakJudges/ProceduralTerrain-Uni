
Texture2D sceneTexture : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	//float3 normal : NORMAL;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
};


float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 t0, t1, t2, t3, t4, t5, t6, t7, t8;
    float4 sobelHorizontal, sobelVertical;
    float4 sobelColour;
	
    //  Sample texture at this pixel and the surrounding pixels.
    t0 = sceneTexture.Sample(Sampler0, input.tex);
    t1 = sceneTexture.Sample(Sampler0, input.texCoord1);
    t2 = sceneTexture.Sample(Sampler0, input.texCoord2);
    t3 = sceneTexture.Sample(Sampler0, input.texCoord3);
    t4 = sceneTexture.Sample(Sampler0, input.texCoord4);
    t5 = sceneTexture.Sample(Sampler0, input.texCoord5);
    t6 = sceneTexture.Sample(Sampler0, input.texCoord6);
    t7 = sceneTexture.Sample(Sampler0, input.texCoord7);
    t8 = sceneTexture.Sample(Sampler0, input.texCoord8);

    //  Detect horizontal colour changes.
    sobelHorizontal = t3 + (2 * t4) + t5 - t1 - (2 * t8) - t7;

    //  Detect vertical colour changes.
    sobelVertical = t7 + (2 * t6) + t5 - t1 - (2 * t2) - t5;

    //  Get the final colour by taking the gradient from the horizontal and vertical directions using pythagoras.
    sobelColour = sqrt(pow(sobelHorizontal, 2) + pow(sobelVertical, 2));

    //  Check the colour of the sobel edge and texture or outline accordingly
    if ((sobelColour.r * 0.3 + sobelColour.g * 0.59 + sobelColour.b * 0.11) < 0.2)
    {
        textureColour = t0;
    }
    else
    {
        textureColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    return textureColour;
}