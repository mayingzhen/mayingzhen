#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------



sampler2D tSrcColor;
sampler2D areaTex;
sampler2D searchTex;

void main(float2 texcoord : TEXCOORD0,
		float2 pixcoord : TEXCOORD1,
        float4 offset[3] : TEXCOORD2,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAABlendingWeightCalculationPS(texcoord,pixcoord,offset,tSrcColor,areaTex,searchTex,0);
}
