// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include "common.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef SKIN
#include "skin.h"
#endif 

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

cbuffer ObjectVS : register(b5)
{
	float4x4 g_matWorldView;
	float4x4 g_matWorldViewProj;
	float4x4 g_matWorld;

	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;
}

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   
   float4 a_normal	 : NORMAL;
   
   float2 a_texCoord0 : TEXCOORD0;
   
#ifdef SKIN   
   uint4   a_blendIndices :BLENDINDICES;
   float4 a_blendWeights :BLENDWEIGHT;     
#endif
   
	//float3 a_tangent  : TANGENT;

};


// Varyings
struct VS_OUT
{
   float2 v_texCoord : TEXCOORD0;

	float4 WorldPos : TEXCOORD1;
	float4 worldNormal : TEXCOORD2;

#ifdef DEFERREDSHADING 
   float3 viewNormal :TEXCOORD3;	
#else  

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float2 oRandDirTC : TEXCOORD4;
	float4 oShadowPos : TEXCOORD5;
#endif
#endif  
   
#ifdef COLOR      
   float4 v_color : COLOR0;
#endif

	float4 v_position : SV_POSITION;
};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
	float3 iPos = In.a_position * pos_extent + pos_center;	
	float2 iUV  = In.a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
	float3 iNormal = In.a_normal.xyz * 2.0 - 1.0;

#ifdef SKIN
	SkinPosNormal(iPos,iNormal,In.a_blendIndices,In.a_blendWeights,iPos,iNormal);
#endif

	float3 WorldPos = mul(float4(iPos,1.0),g_matWorld).xyz;

	Out.v_position = mul(float4(WorldPos.xyz,1.0),g_matViewProj);
   
   Out.WorldPos.xyz = WorldPos;
   Out.WorldPos.w = Out.v_position.w;
   
   Out.worldNormal.xyz = mul(iNormal, (float3x3)g_matWorld);
   Out.worldNormal.w = 0;
	 	
   Out.v_texCoord = iUV;

   
#ifdef COLOR    
    Out.v_color = In.a_color0;
#endif

#ifdef DEFERREDSHADING 
	Out.viewNormal.xyz = mul(iNormal, (float3x3)g_matWorldView); 
#endif

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	GetShadowPos(Out.WorldPos.xyz,Out.oShadowPos);
	#if SHADOW_BLUR == 2 	
	GetRandDirTC(Out.oPos.w,Out.oRandDirTC);  
	#endif	
#endif
  
    return Out;
}




