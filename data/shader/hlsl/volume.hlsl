#include "common.h"

cbuffer ObjectVS : register(b5)
{
	float4x4 matFrustum;
};

void vs_main(float3 iPos			: POSITION0,
		out float4 oPos     : SV_POSITION) 
{

	float4 vPos = float4(iPos,1.0);

	vPos = mul(vPos, matFrustum);

	//normalization
	vPos.xyz /= vPos.w;
	vPos.w = 1.0f;

	//final ViewProj transformation
	oPos = mul(vPos,g_matViewProj); 
}

void ps_main(out float4  out_color0    : SV_TARGET)
{
	out_color0 = float4(1.0,1.0,1.0,1.0);
}
