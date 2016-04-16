#include "common.h"



void main(float3 iPos			: POSITION0,
		out float4 oPos     : POSITION0) 
{

	float4 vPos = float4(iPos,1.0);

	vPos = mul(vPos, g_matWorld);

	//normalization
	vPos.xyz /= vPos.w;
	vPos.w = 1.0f;

	//final ViewProj transformation
	oPos = mul(vPos,g_matViewProj); 
}
