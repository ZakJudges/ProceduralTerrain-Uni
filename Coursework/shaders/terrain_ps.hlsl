

Texture2D grassTexture : register(t0);
Texture2D rockTexture : register(t1);
Texture2D waterTexture : register(t2);
Texture2D snowTexture : register(t3);

SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 lightDirection;
    float specularPower;
    float4 settings;    // x =water enabled, y = voronoi regions enabled, z = cel shading.
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 additionalData : VORONOI;
    float activeRegion : ACTIVE_REGION;
    float waterHeight : WATER_HEIGHT;
    float terrainHeight : TERRAIN_HEIGHT;

};

float4 getToonColour(float4 colour, float diffuseIntensity);

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 grassColour, rockColour, snowColour;
    float3 lightDir;
    float diffuseIntensity;
    float specularIntensity;
    float4 colour;
    float4 specularComponent;
    float4 diffuseComponent;
    float3 halfway;

    //  Lighting calculations:
    float3 normal = input.normal;
	// Invert the light direction for calculations.
    lightDir = -lightDirection;
    //  Set the ambient colour as the defualt amount of light.
    colour = ambient; 
    // Calculate the amount of light on this pixel.
    diffuseIntensity = saturate(dot(normal, lightDir));

    if (diffuseIntensity > 0.0f)
    {
		// Determine the final amount of diffuse colour based on the diffuse colour combined with the light intensity.
        diffuseComponent = saturate(diffuse * diffuseIntensity);
        colour += diffuseComponent; //add the diffuse component to the ambient colour
    }

    //  If water is enabled.
    if (settings.x == 1.0f)
    {
        textureColour = waterTexture.Sample(SampleType, input.tex);
        textureColour.rgb *= 0.65f;
    }
    else
    {
        //  Get the steepness of the slope of the vertex
        float slope = 1.0f - input.normal.y;

        float blendAmount;
        float4 slopeColour;

        //  Calculate what the height of the snow should be at this vertex.
        float snowHeight = 20.0f + (7 * input.normal.y);

        //  Get the colour of the grass, rock and snow textures.
        grassColour = grassTexture.Sample(SampleType, input.tex);
        grassColour.g *= 0.75f;
        rockColour = rockTexture.Sample(SampleType, input.tex);
        snowColour = snowTexture.Sample(SampleType, input.tex);
        
        //  Decide which texture should be applied based on the slope and height of the input vertex.
        if (slope < 0.15)
        {
            textureColour = grassColour;
        }
        if ((slope < 0.65) && (slope >= 0.15f))
        {
            blendAmount = (slope - 0.17f) * 2.0f;
  
            textureColour = lerp(grassColour, rockColour, blendAmount);
        }
        if (slope >= 0.65)
        {
            textureColour = rockColour;
        }

        if(input.terrainHeight > snowHeight)
        {
            if(slope < 0.25f)
            {
                textureColour = snowColour;
            }
            if(slope > 0.25 && slope < 0.7)
            {
                //blendAmount = (slope - 0.17f) * 2.0f;
                blendAmount = (1.0f - slope) + slope;
                textureColour = lerp(snowColour, rockColour, blendAmount);
            }
        }

        //  If the water is above the vertex then set the colour of the pixel towards the rock colour.
        if (input.terrainHeight < (input.waterHeight - 0.2f))
        {
            textureColour = lerp(textureColour, rockColour, 0.35f);
        }
    }
  
    //  Multiply the texture pixel and the final diffuse colour to get the final pixel colour result.
    colour = colour * textureColour;

    //  If voronoi regions is enabled then lighten the selected region.
    if(settings.y == 1.0f)
    {
        //  If the vertex voronoi point is in the active region, then lighten the pixel.
        if (input.additionalData.x == input.activeRegion)
        {
            colour.rgb *= 1.5f;
        }
    }

    //  If toon shading is enabled then get the toon colour.
    if(settings.z == 1.0f)
    {
        return getToonColour(colour, diffuseIntensity);
    }
    else
    {
        return colour;
    }
}

float4 getToonColour(float4 colour, float diffuseIntensity)
{
    if (diffuseIntensity > 0.95)
    {
        colour *= float4(1.0f, 1.0f, 1.0f, 1.0f);

    }
    else if (diffuseIntensity > 0.5)
    {
        colour *= float4(0.8f, 0.8f, 0.8f, 1.0f);

    }
    else if (diffuseIntensity > 0.05)
    {
        colour *= float4(0.6f, 0.6f, 0.6f, 1.0f);

    }
    else
    {
        colour *= float4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    return colour;
}
