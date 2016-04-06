#include"common.h"
#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------
texture mtTexture0;
sampler tSrcColor
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_LINEAR";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtTexture0);
};


void main(float2 texcoord      : TEXCOORD0,
		float4 offset[3]       : TEXCOORD1,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAAColorEdgeDetectionPS(texcoord,offset,tSrcColor);
}

