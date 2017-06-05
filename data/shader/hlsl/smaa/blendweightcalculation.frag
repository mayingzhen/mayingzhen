#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------



Texture2D tSrcColor;
Texture2D areaTex;
Texture2D searchTex;

void main(float2 texcoord : TEXCOORD0,
		float2 pixcoord : TEXCOORD1,
        float4 offset[3] : TEXCOORD2,
        
        out float4 oColor   :SV_TARGET)
{
	oColor = SMAABlendingWeightCalculationPS(texcoord,pixcoord,offset,tSrcColor,areaTex,searchTex,0);
}
