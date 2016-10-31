
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


Texture2D tMask : register(t1);
SamplerState sMask : register(s1);

Texture2D tSpecularRMCMap : register(t2);
SamplerState sSpecularRMCMap : register(s2);

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
   
    cDiffuse *= u_texture.Sample(s_texture, In.v_texCoord).xyz;
    
    float roughness = u_roughness;
    float metalness = u_metalness;
    float3 specColor = u_cSpecColor.xyz;
    
#ifdef MASK_TEXTURE    
    //float4 mask = tMask.Sample(sMask,In.v_texCoord);
    //roughness = exp2((mask.g) * 13);
	//specColor = float3(mask.x,mask.x,mask.x);
#endif    
    
	
	float3 vNormal = normalize(In.worldNormal.xyz);
	//vNormal.yz = vNormal.zy;
	float3 vView  = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
	
    // Specular map parameters.
#ifdef MASK_TEXTURE  
    float4 specularRMC = tSpecularRMCMap.Sample(sSpecularRMCMap,In.v_texCoord);
    roughness = 1.0 - specularRMC.x;
    metalness = specularRMC.y;
#endif     
			
#if DEFERREDSHADING == 0
	oColor.xyz = ForwardLighing(cDiffuse,specColor,roughness,metalness,vView,vNormal);
#else
	oNormal = 0;
	float3 viewNormal = mul(oWorldNormal, (float3x3)g_matView); 
	oNormal.xy = EncodeNormal( normalize(viewNormal.xyz) );
	const float3 RGB_TO_LUM = float3(0.299f, 0.587f, 0.114f);
	#ifdef SPEC
		oNormal.z = dot(u_cSpecColor.xyz,RGB_TO_LUM) ;
		oNormal.w = u_specPower / 255.0f;
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
