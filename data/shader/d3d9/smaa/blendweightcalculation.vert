#include"smaa.h"

void main(float4 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : POSITION0,
        out float2 texcoord : TEXCOORD0,
		out float2 pixcoord : TEXCOORD1,
        out float4 offset[3] : TEXCOORD2
)
{
	oPos = iPos;
	texcoord = iUV;
	
	SMAABlendingWeightCalculationVS(texcoord,pixcoord,offset);
}

