
#include "common.h"
#include "lighting.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

Texture2D u_texture : register(t0);
SamplerState s_texture : register(s0);


// Varyings	
struct PS_IN
{
   float2   v_texCoord : TEXCOORD0;

	float4 WorldPos : TEXCOORD1;
	float4 worldNormal :TEXCOORD2;

#ifdef DEFERREDSHADING 
   float3 viewNormal  :TEXCOORD3;	
#else  

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float2 oRandDirTC : TEXCOORD4;
	float4 oShadowPos : TEXCOORD5;
#endif
#endif 
   
};


void main(PS_IN In,
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
