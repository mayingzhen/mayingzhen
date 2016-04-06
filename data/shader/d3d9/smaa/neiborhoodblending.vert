#include"common.h"
#include"smaa.h"


void main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : POSITION0,
        out float2 texcoord : TEXCOORD0,
        out float4 offset   : TEXCOORD1
)
{
	SMAANeighborhoodBlendingVS(texcoord,offset);
}

