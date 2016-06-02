
#include "common.h"
#include "lighting.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

Texture2D u_texture : register(t0);
SamplerState s_texture : register( s0 );





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
out float4 oColor : SV_TARGET0
#if DEFERREDSHADING == 1
,out float4 oNormal : SV_TARGET1
#endif
) 
{
   float3 cDiffuse = u_cDiffuseColor.xyz;	
   oColor.a = u_cDiffuseColor.w;
   
   cDiffuse *= u_texture.Sample(s_texture, In.v_texCoord);
	
	oColor.xyz = cDiffuse.xyz;
	
	float3 vNormal = normalize(In.worldNormal.xyz);
		
#if DEFERREDSHADING == 0

#ifdef LIGHT
	float3 vView  = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
	float3 vlight = normalize(g_vDirLight.xyz);
	float3 halfVec = normalize(vView + vlight);
	
	#ifdef BRDF
		#ifdef SPEC
			oColor.rgb = calc_brdf(cDiffuse,u_cSpecColor,u_roughness,vlight,halfVec,vNormal);
		#else
			oColor.rgb = calc_brdf(cDiffuse,vlight,vNormal);
		#endif		
	#else
		float4 light = lit( dot( vNormal, vlight ), dot( vNormal, halfVec ), u_roughness );   
		float3 Diffuse = light.y * cDiffuse * g_cDirLight.xyz;
		float3 Specular = light.z * u_cSpecColor.xyz * g_cDirLight.xyz;;			
		oColor.xyz = g_cSkyLight + Diffuse;	
			
		#ifdef SPEC			
			oColor.xyz += Specular * g_cDirLight.xyz;	
		#endif 
    #endif 
#endif  

    #ifdef ENVREFLECT
		float envroughness = log2(u_roughness) / 13; // log2(8192) == 13
		float3 envDiffuse = PrefilteredDiffuseIBL(cDiffuse.xyz, vNormal);
		float3 envSpec = PBFittingPrefilteredSpecularIBL(u_cSpecColor.xyz, envroughness, vNormal, vView);
		
		oColor.xyz += envDiffuse + envSpec;	
    #endif

#endif   

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	#ifdef RECEIVESHADOW
		float4 ShadowPos = In.oShadowPos;
		float2 RandDirTC = 0;
		#if SHADOW_BLUR == 2
			RandDirTC = In.oRandDirTC;
		#endif
		oColor.rgb *= DoShadowMapping(ShadowPos,RandDirTC,In.WorldPos.w);		
	#endif
#endif
}
