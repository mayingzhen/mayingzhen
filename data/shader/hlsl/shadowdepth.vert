#include"common.h"
#include "skin.h"

cbuffer ObjectVS : register(b5)
{
	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;
	
	float4x4	g_matWorld;	
	float4x4	matLightViewProj;
}

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   float4 a_normal : NORMAL;
   float2 a_texCoord : TEXCOORD0; 

#ifdef SKIN   
   int4   a_blendIndices :BLENDINDICES;
   float4 a_blendWeights :BLENDWEIGHT;     
#endif
};

struct VS_OUT
{
	float2 oUV	: TEXCOORD0;
		
#if USING_HW_PCF == 0
	float4 oDepth : TEXCOORD1;
#endif	

	float4 oPos : SV_POSITION;
};




VS_OUT main( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
 
	float3 iPos = In.a_position * pos_extent + pos_center;
	float2 iUV  = In.a_texCoord * tc_extent_center.xy + tc_extent_center.zw;

#ifdef SKIN
	float3 finalPos = SkinPos(iPos,In.a_blendIndices,In.a_blendWeights);
#else
	float3 finalPos = iPos;		
#endif
	
	float4 worldPos = mul(float4(finalPos,1.0f), g_matWorld);
		
#ifdef WINDVERTEX	
	WindVertex(g_matWorld, worldPos, In.a_position, In.a_normal);
#endif

	Out.oPos = mul(worldPos, matLightViewProj);
	
	Out.oUV = iUV;

#if USING_HW_PCF == 0
	Out.oDepth = Out.oPos;  
#endif	

	return Out;  
}








