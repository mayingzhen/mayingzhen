#include"common.h"
#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------


texture mtedgesTex;
sampler tSrcColor
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_LINEAR";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtedgesTex);
};

texture mtTexture1;
sampler areaTex
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_LINEAR";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
	string TextureAddressingModeW = "TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtTexture1);
};

texture mtTexture2;
sampler searchTex
<
	string TextureFiltering = "FO_POINT,FO_POINT,FO_POINT";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
	string TextureAddressingModeW = "TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtTexture2);
};


void main(float2 texcoord : TEXCOORD0,
		float2 pixcoord : TEXCOORD1,
        float4 offset[3] : TEXCOORD2,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAABlendingWeightCalculationPS(texcoord,pixcoord,offset,tSrcColor,areaTex,searchTex,0);
}
