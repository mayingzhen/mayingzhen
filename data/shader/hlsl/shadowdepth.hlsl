#include"common.h"
#include "skin.h"

cbuffer ObjectVS : register(b5)
{
	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;
	
	float4x4	g_matWorld;	
	float4x4	matLightViewProj;
};

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   float4 a_normal : NORMAL;
   float2 a_texCoord : TEXCOORD0; 

#ifdef SKIN   
   uint4   a_blendIndices :BLENDINDICES;
   float4 a_blendWeights :BLENDWEIGHT;     
#endif

#ifdef INSTANCE
	float4 a_texCoord1 : TEXCOORD1;
	float4 a_texCoord2 : TEXCOORD2;
	float4 a_texCoord3 : TEXCOORD3;
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




VS_OUT vs_main( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
 
	float3 iPos = In.a_position * pos_extent + pos_center;
	float2 iUV  = In.a_texCoord * tc_extent_center.xy + tc_extent_center.zw;

#ifdef SKIN
	float3 finalPos = SkinPos(iPos,In.a_blendIndices,In.a_blendWeights);
#else
	float3 finalPos = iPos;		
#endif

#ifdef INSTANCE
	float4x4 matWorld = float4x4(float4(In.a_texCoord1.x, In.a_texCoord2.x, In.a_texCoord3.x, 0.0),
						float4(In.a_texCoord1.y, In.a_texCoord2.y, In.a_texCoord3.y, 0.0),
						float4(In.a_texCoord1.z, In.a_texCoord2.z, In.a_texCoord3.z, 0.0),
						float4(In.a_texCoord1.w, In.a_texCoord2.w, In.a_texCoord3.w, 1.0));
	//float4x4 matWorld = float4x4(In.a_texCoord1,
	//							 In.a_texCoord2,
	//							 In.a_texCoord3,
	//							 float4(0.0, 0.0, 0.0, 1.0));
#else
	float4x4 matWorld = g_matWorld;
#endif
	
	float4 worldPos = mul(float4(finalPos,1.0f), matWorld);
		
#ifdef WINDVERTEX	
	WindVertex(matWorld, worldPos, In.a_position, In.a_normal);
#endif

	Out.oPos = mul(worldPos, matLightViewProj);
	
	Out.oUV = iUV;

#if USING_HW_PCF == 0
	Out.oDepth = Out.oPos;  
#endif	

	return Out;  
}



Texture2D tDiff : register(t0);
SamplerState sDiff : register( s0 );


float4 OutPutDepth(float fDepth)
{
	fDepth += 0.0010f;

#if USING_FLOATTEXTURE == 1
	return fDepth;
#else
	return EncodeFloatRGBA(fDepth);
#endif	
}

float4 ps_main(VS_OUT In) : SV_TARGET
{
	float4 oColor = 0;
#ifdef ALPHATEST
    oColor.a = tDiff.Sample(sDiff, In.oUV).a;
    clip(oColor.a - 0.5);
#endif

#if USING_HW_PCF == 0
	oColor = OutPutDepth(In.oDepth.z / In.oDepth.w);
#else
	oColor = 0.0;
#endif

	return oColor;
}


