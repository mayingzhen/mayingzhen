#include"smaa.h"


// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------

void main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : POSITION0,
        out float2 texcoord : TEXCOORD0,
        out float4 offset[3]: TEXCOORD1
)
{
    oPos = float4(iPos.x-SMAA_RT_METRICS.x, iPos.y+SMAA_RT_METRICS.y, 0.5, 1);
    texcoord = iUV;
    
	SMAAEdgeDetectionVS(texcoord,offset);
}


