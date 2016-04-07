#include"smaa.h"


void main(float4 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : POSITION0,
        out float2 texcoord : TEXCOORD0,
        out float4 offset   : TEXCOORD1
)
{
	oPos = iPos;
	texcoord = iUV;
	
	SMAANeighborhoodBlendingVS(texcoord,offset);
}

