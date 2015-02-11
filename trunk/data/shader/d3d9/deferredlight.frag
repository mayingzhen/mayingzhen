#include "common.h"

#ifdef POINT_LIGHT   
uniform float4 light_pos_es_radius;
#endif

#ifdef DIRECT_LIGHT
uniform float3 light_dir_es;
#endif

uniform float4 light_color ;//= (0.2f,0.2f,0.2f,0.2f);



// Gbuffer
sampler2D u_textureSceneDiffuse;
sampler2D u_textureSceneDepth;
sampler2D u_textureSceneNormal;

// shadow
#if USING_SHADOW != 0
sampler2D u_TextureSceneShadow;
#endif
      
struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};

struct PS_OUT
{
   //float4 Diffuse  : COLOR0;
   //float4 Specular  : COLOR1;
   float4 flagColor : COLOR0;
};



float GetLinearDepth(float2 tc)
{
#ifdef HWDEPTH
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tex2D(u_textureSceneDepth, tc).r;
	depth = g_vCameraNearFar.x / (q - depth);
#else
	float depth = tex2D(u_textureSceneDepth, tc).r;
    depth *= g_vCameraNearFar.y;
#endif	
	
	return depth;
}



void GetPosNormalShiness(VS_OUT In,out float3 pos_es,out float3 normal,out float shiness)
{
   float depth = GetLinearDepth(In.oTc); 
   
   float3 view_dir = normalize(In.oViewDir);
   pos_es = view_dir * (-depth / view_dir.z); 
   
   float4 SrcNormal = tex2D( u_textureSceneNormal, In.oTc);
   
#ifdef ENCODENORMAL 
   normal = DecodeNormal(SrcNormal.xy);
#else   
   normal = SrcNormal.xyz;
#endif 
 
   shiness = SrcNormal.w  * 255.0f;
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
   float shiness;
   GetPosNormalShiness(In,pos_es,normal,shiness);

#ifdef POINT_LIGHT   
   float3 vlightVec = light_pos_es_radius.xyz - pos_es.xyz;
#else 
#ifdef DIRECT_LIGHT
   float3 vlightVec = light_dir_es.xyz;      
#endif
#endif  

	float3 	Diffuse = 0;
	float3 	Specular = 0;
   GetDiffuseSpecular(vlightVec,pos_es,normal,shiness,Diffuse,Specular);
   
   float4 mdiffuse = tex2D( u_textureSceneDiffuse, In.oTc);  		 

   pOut.flagColor.xyz = Diffuse * mdiffuse + Specular;
   pOut.flagColor.w = 1.0;
}
#endif


void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   //pOut.Diffuse.xyz = light_color;
  // pOut.Specular.xyz = 0; 
  float4 mdiffuse = tex2D( u_textureSceneDiffuse, In.oTc);  
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
   float shadow = tex2D(u_TextureSceneShadow, vout.oTc).r;
   
   pout.flagColor *= shadow;
#endif

#endif
}

