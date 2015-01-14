#include"common.h"
#include "skin.h"

float4x4	matLightViewProj;

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   float2 a_texCoord : TEXCOORD0; 
   float3 a_normal : NORMAL0;

#ifdef SKIN   
   float4 a_blendWeights :BLENDWEIGHT;      
   int4   a_blendIndices :BLENDINDICES;
#endif   
};

struct VS_OUT
{
	float4 oPos : POSITION;
	
	float2 oUV	: TEXCOORD0;
		
#ifndef HWPCF
	float4 oDepth : TEXCOORD1;
#endif	
};




VS_OUT main( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
 
	float3 finalPos  = In.a_position;

#ifdef SKIN
	finalPos  =  SkinPos(In.a_position,In.a_blendIndices,In.a_blendWeights);
#endif
	
	float4 worldPos = mul(float4(finalPos,1.0f), g_matWorld);
		
#ifdef WINDVERTEX	
	WindVertex(g_matWorld, worldPos, In.a_position, In.a_normal);
#endif

	Out.oPos = mul(worldPos, matLightViewProj);
	
	Out.oUV = In.a_texCoord;

#ifndef HWPCF
	Out.oDepth = Out.oPos;  
#endif	

	return Out;  
}








