#include "common.h"
#include "depth.h"
#include "lighting.h"

//#define DIRECT_LIGHT 1

cbuffer ObjectPS : register(b5)
{
	#ifdef POINT_LIGHT   
	uniform float4 light_pos_radius;
	#endif

	#ifdef DIRECT_LIGHT
	uniform float3 light_dir;
	#endif

	float4 light_color ;
};


// Gbuffer
Texture2D u_textureSceneDiffuse : register(t5);
SamplerState s_textureSceneDiffuse : register(s5);

Texture2D u_textureSceneNormal  : register(t6);
SamplerState s_textureSceneNormal : register(s6);

// shadow
#if USING_SHADOW != 0
Texture2D u_TextureSceneShadow : register(t7);
SamplerState s_TextureSceneShadow : register(s7);
#endif
      
struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
};

struct PS_OUT
{
   float4 flagColor : SV_TARGET;
};



void GetPosNormalShiness(VS_OUT In,out float3 pos,out float3 normal,out float metalness,out float glossiness,out float3 albedo)
{
   float depth = GetLinearDepth(In.oTc); 
   
   float3 view_dir = normalize(In.oViewDir.xyz);
   float3 pos_es = view_dir * (depth / view_dir.z); 

   pos = mul(float4(pos_es,1.0),g_matViewInv).xyz;
   
   float4 SrcNormal = u_textureSceneNormal.Sample(s_textureSceneNormal, In.oTc);

   float4 baseColor = u_textureSceneDiffuse.Sample(s_textureSceneDiffuse, In.oTc); 

 	metalness = baseColor.w;

	glossiness = SrcNormal.w;
 
   albedo = baseColor.rgb;

   normal = SrcNormal.xyz * 2.0f - 1.0f;
   normal = normalize(normal);
}


#ifndef AMBIENT_LIGHT
void DeferredLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   float3 vWorldPos;
   float3 vWorldNormal;
   float metalness;
   float glossiness;
   float3 albedo;
   GetPosNormalShiness(In,vWorldPos,vWorldNormal,metalness,glossiness,albedo);

   float3 vView = normalize(g_vEyeWorldPos.xyz - vWorldPos.xyz);

   float3 vlightVec = float3(0.0f,0.0f,0.0f); 

#ifdef POINT_LIGHT   
   vlightVec = light_pos_radius.xyz - vWorldPos.xyz;
   float attenuation = saturate(1.0f - length(vlightVec) / light_pos_radius.w); 
#else 
#ifdef DIRECT_LIGHT
   vlightVec = light_dir.xyz;
   float attenuation = 1.0f;       
#endif
#endif  

   vlightVec = normalize(vlightVec);

   
   float3 diffColor = albedo.rgb * (1.0 - metalness);
	float3 specColor = lerp(0.04, albedo.rgb, metalness); 

	float3 light = LightBRDF(glossiness,light_color.xyz,vlightVec.xyz,
		vWorldNormal.xyz,vView.xyz,diffColor,specColor,1.0f,1.0f) * attenuation;

   pOut.flagColor.xyz = light;
   pOut.flagColor.w = 1.0;
}
#endif


void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
  float4 mdiffuse = u_textureSceneDiffuse.Sample(s_textureSceneDiffuse, In.oTc);  
  pOut.flagColor = light_color * mdiffuse;
}


void main( VS_OUT vout, out PS_OUT pout ) 
{
#ifdef AMBIENT_LIGHT
   AmbientLightPS(vout,pout);  
   return;
#else  

   DeferredLightPS(vout,pout);   
   
#if USING_SHADOW != 0
   float shadow = u_TextureSceneShadow.Sample(s_TextureSceneShadow, vout.oTc).r;
   
   pout.flagColor *= shadow;
#endif

#endif
}

