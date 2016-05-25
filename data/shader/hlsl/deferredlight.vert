#include "common.h"

#ifdef AMBIENT_LIGHT
#define SCEERN_LIGHT
#endif

#ifdef DIRECT_LIGHT
#define SCEERN_LIGHT
#endif

   
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
}


void main(
#ifdef SCEERN_LIGHT
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
#else
	float3 pos : POSITION,
#endif
    out VS_OUT vOut)
{
#ifdef SCEERN_LIGHT
   vOut.pos = float4(pos.x, pos.y, 0.0, 1);
   vOut.oViewDir = mul(float4(vOut.pos.xyz ,1),g_matProjInv);
   vOut.oTc = iUV;
#else
   float3 iPos = pos * pos_extent + pos_center;	
   vOut.pos =  mul(float4(iPos.xyz ,1),g_matWorldViewProj);
   vOut.oViewDir =  mul(float4(pos.xyz ,1),g_matWorldView);
   vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
   vOut.oTc.y *= -1;
   vOut.oTc += 0.5f;
#endif
}








