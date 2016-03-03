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

float3 pos_extent;
float3 pos_center;
float4 tc_extent_center;

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;
   
#ifdef SKIN   
   int4   a_blendIndices :BLENDINDICES;
   float4 a_blendWeights :BLENDWEIGHT;     
#endif
   
   float4 a_normal	 : NORMAL;
 
#ifdef DIFFUSE   
   float2 a_texCoord0 : TEXCOORD0;
#endif

	float3 a_tangent  : TANGENT;

#ifdef COLOR   
   float4 a_color0 : COLOR0;
#endif

};


// Varyings
struct VS_OUT
{
   float4 v_position : POSITION;

#ifdef DIFFUSE    
   float2 v_texCoord : TEXCOORD0;
#endif  

	float4 WorldPos : TEXCOORD1;

#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD2;	
#else  

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float2 oRandDirTC : TEXCOORD2;
	float4 oShadowPos : TEXCOORD3;
#endif
#endif  
   
#ifdef COLOR      
   float4 v_color : COLOR0;
#endif

};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
	float3 iPos = In.a_position * pos_extent + pos_center;
#ifdef DIFFUSE  	
	float2 iUV  = In.a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
#endif	
	float3 iNormal = In.a_normal * 2.0 - 1.0; 

#ifdef SKIN
	SkinPosNormal(iPos,iNormal,In.a_blendIndices,In.a_blendWeights,iPos,iNormal);
#endif
   
   Out.WorldPos.xyz = mul(float4(iPos,1.0),g_matWorld).xyz;
   
    Out.v_position = mul(float4(Out.WorldPos.xyz,1.0),g_matViewProj); 
	
	Out.WorldPos.w = Out.v_position.w;
	 
#ifdef DIFFUSE      
    Out.v_texCoord = iUV;
#endif
   
#ifdef COLOR    
    Out.v_color = In.a_color0;
#endif

#ifdef DEFERREDSHADING 
	GBufferVSOut(iNormal,Out.v_position.w,Out.v_normalDepth);
#else
#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	GetShadowPos(Out.WorldPos.xyz,Out.oShadowPos);
	#if SHADOW_BLUR == 2 	
	GetRandDirTC(Out.oPos.w,Out.oRandDirTC);  
	#endif	
#endif
#endif
  
    return Out;
}




