#include"common.h"
#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------

texture mtTexture0;
sampler tSrcColor
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_POINT";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
	string TextureSRGB = "true";
>
 = sampler_state
{
    Texture = (mtTexture0);
};

texture mtTexture1;
sampler blendTex
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_LINEAR";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtTexture1);
};

void main(float2 texcoord      : TEXCOORD0,
		float4 offset          : TEXCOORD1,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAANeighborhoodBlendingPS(texcoord,offset,tSrcColor,blendTex);
}
