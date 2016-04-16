#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------
sampler2D tSrcColor;


void main(float2 texcoord      : TEXCOORD0,
		float4 offset[3]	 : TEXCOORD1,
        
        out float4 oColor   :COLOR0)
{
	oColor = 0;
	oColor.xy = SMAADepthEdgeDetectionPS(texcoord,offset,tSrcColor);
}
