#include"smaa.h"

void main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : POSITION0,
        out float2 texcoord : TEXCOORD0,
		out float2 pixcoord : TEXCOORD1,
        out float4 offset[3] : TEXCOORD2
)
{
	oPos = float4(iPos.x-SMAA_RT_METRICS.x, iPos.y+SMAA_RT_METRICS.y, 0.5, 1);
	texcoord = iUV;
	
	SMAABlendingWeightCalculationVS(texcoord,pixcoord,offset);
}

