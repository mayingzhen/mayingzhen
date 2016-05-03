#include"smaa.h"


void main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float2 texcoord : TEXCOORD0,
        out float4 offset   : TEXCOORD1,
        out float4 oPos     : SV_POSITION
)
{
	oPos = float4(iPos.x, iPos.y, 1.0, 1);
	texcoord = iUV;
	
	SMAANeighborhoodBlendingVS(texcoord,offset);
}

