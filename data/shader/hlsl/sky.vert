#include "common.h"

cbuffer ObjectVS : register(b5)
{
	float4x4 g_matWorldView;
	float4x4 g_matWorldViewProj;
	float4x4 g_matWorld;

	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;
	
	float u_speed;
}

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   float2 a_texCoord0 : TEXCOORD0;
};


// Varyings
struct VS_OUT
{
   float4 texCoord0 : TEXCOORD0;
   float4 texCoord1 : TEXCOORD1;
   
   float4 v_position : SV_POSITION;
};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
    float3 iPos = In.a_position * pos_extent + pos_center;	
	float2 iUV  = In.a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
    
    Out.v_position = mul(float4(iPos,1.0),g_matWorldViewProj).xyz;
    
	Out.texCoord0.xy = g_fTime * 1.0 * u_speed * float2(1.0, 0.0);
	Out.texCoord0.zw = g_fTime * 1.5 * u_speed * float2(0.0, 1.0);
	Out.texCoord1.xy = g_fTime * 2.0 * u_speed * float2(0.0, -1.0);
	Out.texCoord1.zw = g_fTime * 2.5 * u_speed * float2(-1.0, 0.0);
	
	return Out;			
}