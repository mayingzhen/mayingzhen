#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------


Texture2D tSrcColor;
Texture2D blendTex;

void main(float2 texcoord      : TEXCOORD0,
		float4 offset          : TEXCOORD1,
        
        out float4 oColor   :SV_TARGET)
{
	oColor = SMAANeighborhoodBlendingPS(texcoord,offset,tSrcColor,blendTex);
}
