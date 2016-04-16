#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------


sampler2D tSrcColor;
sampler2D blendTex;

void main(float2 texcoord      : TEXCOORD0,
		float4 offset          : TEXCOORD1,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAANeighborhoodBlendingPS(texcoord,offset,tSrcColor,blendTex);
}
