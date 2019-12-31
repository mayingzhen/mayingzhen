#include "common.h"

   
struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
   float4 pos : SV_POSITION;
};

cbuffer ObjectVS : register(b5)
{
	float4x4 g_matWorldViewProj;
	float4x4 g_matWorldView;
	
	float3 pos_extent;
	float3 pos_center;
};


void main(
#if defined(AMBIENT_LIGHT) || defined(DIRECT_LIGHT)
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
#else
	float3 pos : POSITION,
#endif
    out VS_OUT vOut)
{
#if defined(AMBIENT_LIGHT) || defined(DIRECT_LIGHT)
   vOut.pos = float4(pos.x, pos.y, 0.0, 1);
   vOut.oViewDir = mul(float4(vOut.pos.xyz ,1),g_matProjInv);
   vOut.oTc = iUV;
#else
   //float3 iPos = pos * pos_extent + pos_center;	
   vOut.pos =  mul(float4(pos.xyz ,1),g_matWorldViewProj);
   vOut.oViewDir =  mul(float4(pos.xyz ,1),g_matWorldView);
   vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
   vOut.oTc.y *= -1;
   vOut.oTc += 0.5f;
#endif
}








