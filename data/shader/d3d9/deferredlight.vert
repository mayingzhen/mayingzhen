#include "common.h"

#ifdef AMBIENT_LIGHT
#define SCEERN_LIGHT
#endif

#ifdef DIRECT_LIGHT
#define SCEERN_LIGHT
#endif

   
struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};



void main( float3 pos : POSITION,
                 out VS_OUT vOut)
{
#ifdef SCEERN_LIGHT
   vOut.pos = float4(pos.xyz ,1);
   vOut.oViewDir = mul(float4(pos.xyz ,1),g_matProjInv).xyz;
#else
   vOut.pos =  mul(float4(pos.xyz ,1),g_matWorldViewProj);
   vOut.oViewDir =  mul(float4(pos.xyz ,1),g_matWorldView).xyz;
#endif
   
   vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
   vOut.oTc.y *= -1;
   vOut.oTc += 0.5f;
}








