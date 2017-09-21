#include "common.h"
#include "depth.h"

cbuffer ObjectPS : register(b5)
{
	#ifdef POINT_LIGHT   
	uniform float4 light_pos_es_radius;
	#endif

	#ifdef DIRECT_LIGHT
	uniform float3 light_dir_es;
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



void GetPosNormalShiness(VS_OUT In,out float3 pos_es,out float3 normal,out float specPower,out float specIntensity)
{
   float depth = GetLinearDepth(In.oTc); 
   
   float3 view_dir = normalize(In.oViewDir.xyz);
   pos_es = view_dir * (depth / -view_dir.z); 
   
   float4 SrcNormal = u_textureSceneNormal.Sample(s_textureSceneNormal, In.oTc);
   
#ifdef ENCODENORMAL 
   normal = DecodeNormal(SrcNormal.xy);
   specIntensity = SrcNormal.z;
#else   
   normal = SrcNormal.xyz;
   specIntensity = 0.0f;
#endif 
 
   specPower = SrcNormal.w  * 255.0f;
}

void GetDiffuseSpecular(float3 lightVec, float3 pos_es, float3 normal,float shiness,out float3 Diffuse,out float3 Specular)
{
	Diffuse = 0;
	Specular = 0;
	
   float3 vNormal = normalize(normal);
   float3 vLight  = normalize(lightVec);   
   float3 vView   = -normalize(pos_es.xyz);
   float3 vHalfDir = normalize(vView + vLight);
   
   float4 light = lit( dot( vNormal, vLight ), dot( vNormal, vHalfDir ), shiness );   
   
   float3 cDiffUse = light.y * light_color;
   float3 cSpecular = light.z * light_color;

#ifdef POINT_LIGHT      
   float attenuation = saturate(1.0f - length(lightVec)/light_pos_es_radius.w); 
#else 
   float attenuation = 1.0f; 
#endif       

  Diffuse.xyz = attenuation * cDiffUse;
  Specular.xyz = attenuation * cSpecular;
}


#ifndef AMBIENT_LIGHT
void DeferredLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   float3 pos_es;
   float3 normal;
   float specPower;
   float specIntensity;
   GetPosNormalShiness(In,pos_es,normal,specPower,specIntensity);

#ifdef POINT_LIGHT   
   float3 vlightVec = light_pos_es_radius.xyz - pos_es.xyz;
#else 
#ifdef DIRECT_LIGHT
   float3 vlightVec = light_dir_es.xyz;      
#endif
#endif  

	float3 	Diffuse = 0;
	float3 	Specular = 0;
   GetDiffuseSpecular(vlightVec,pos_es,normal,specPower,Diffuse,Specular);
	
   float4 mdiffuse = u_textureSceneDiffuse.Sample(s_textureSceneDiffuse, In.oTc);  		 

   pOut.flagColor.xyz = Diffuse * mdiffuse.xyz + Specular * specIntensity;
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

