
#include "common.h"
#include "lighting.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

#ifdef SKIN
#include "skin.h"
#endif 

Texture2D u_texture : register(t0);
SamplerState s_texture : register(s0);


cbuffer ObjectVS : register(b5)
{
	float4x4 g_matWorldView;
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

#ifdef INSTANCE
	float4 a_texCoord1 : TEXCOORD1;
	float4 a_texCoord2 : TEXCOORD2;
	float4 a_texCoord3 : TEXCOORD3;
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



VS_OUT vs_main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
  
	float3 iPos = In.a_position * pos_extent + pos_center;	
	float2 iUV  = In.a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
	float3 iNormal = In.a_normal.xyz * 2.0 - 1.0;

#ifdef SKIN
	SkinPosNormal(iPos,iNormal,In.a_blendIndices,In.a_blendWeights,iPos,iNormal);
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

	float3 WorldPos = mul(float4(iPos,1.0),matWorld).xyz;

	Out.v_position = mul(float4(WorldPos.xyz,1.0),g_matViewProj);
   
   Out.WorldPos.xyz = WorldPos;
   Out.WorldPos.w = Out.v_position.w;
   
   Out.worldNormal.xyz = mul(iNormal, (float3x3)matWorld);
   Out.worldNormal.w = 0;
	 	
   Out.v_texCoord = iUV;

   
#ifdef COLOR    
    Out.v_color = In.a_color0;
#endif

#ifdef DEFERREDSHADING 
	Out.viewNormal.xyz = mul(Out.worldNormal.xyz, (float3x3)g_matView); 
#endif

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	GetShadowPos(Out.WorldPos.xyz,Out.oShadowPos);
	#if SHADOW_BLUR == 2 	
	GetRandDirTC(Out.oPos.w,Out.oRandDirTC);  
	#endif	
#endif
  
    return Out;
}


void ps_main(VS_OUT In,
out DRMRTOut mrtOut
) 
{
	InitMRTOutPut(mrtOut);

	float4 oColor = 0;

	float4 albedo = 0;
	
	albedo = u_cDiffuseColor;	
   
    albedo *= u_texture.Sample(s_texture, In.v_texCoord);
    
	float3 vNormal = normalize(In.worldNormal.xyz);
	float3 vView  = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
	
		// ��Ӱ
	float fShadowMapShadow = 1.0;
#if USING_SHADOW != 0  && USING_DEFERREDSHADOW == 0
	#ifdef RECEIVESHADOW
		float4 ShadowPos = In.oShadowPos;
		float2 RandDirTC = 0;
			#if SHADOW_BLUR == 2
				RandDirTC = In.oRandDirTC;
			#endif
		fShadowMapShadow = DoShadowMapping(ShadowPos,RandDirTC,oWorldPos.w);
	#endif		
#endif
	    
	float metalness = 0;
	float glossiness = 0;
	GetMetalnessGlossiness(In.v_texCoord,metalness,glossiness,float2(0,0),0);

#if DEFERREDSHADING == 0
	mrtOut.oColor.rgb = ForwardPixelLighting(metalness,glossiness,vNormal,vView,albedo.rgb,fShadowMapShadow,1.0);
#else
	FinalMRTOutPut(metalness,glossiness,albedo.rgb,vNormal,mrtOut);	
#endif

}
