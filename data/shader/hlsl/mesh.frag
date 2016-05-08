
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


cbuffer ObjectPS : register(b5)
{
	float4 u_cDiffuseColor;

	float4 u_cSpecColor;
	float u_specPower;
}



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
   
#ifdef COLOR      
   float4   v_color : COLOR0;
#endif   
};

float4 GetDiffuse(PS_IN In)
{
#ifdef DIFFUSECOLOR   
    float4 flagColor = u_cDiffuseColor;
#else
	float4 flagColor = float4(1,1,1,1);
#endif   
   
#ifdef COLOR   
   flagColor = In.v_color;   
#endif

   flagColor *= u_texture.Sample(s_texture, In.v_texCoord);

	return flagColor;
}

#if DEFERREDSHADING == 0
float4 ForwardShading(float4 cDiffuse,PS_IN In)
{
	float4 flagColor = cDiffuse;
	
	float3 vNormal = normalize(In.worldNormal.xyz);


#ifdef LIGHT
	float3 vView  = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
	float3 vlight = normalize(g_vDirLight.xyz);
	float3 halfVec = normalize(vView + vlight);
	
	#ifdef BRDF
		#ifdef SPEC
			flagColor.rgb = calc_brdf(cDiffuse,u_cSpecColor,u_specPower,vlight,halfVec,vNormal);
		#else
			flagColor.rgb = calc_brdf(cDiffuse,vlight,vNormal);
		#endif		
	#else
		float4 light = lit( dot( vNormal, vlight ), dot( vNormal, halfVec ), u_specPower );   
		
		float3 Diffuse = light.y * g_cDirLight.xyz;
		float3 Specular = light.z * g_cDirLight.xyz;	
			
		flagColor.xyz = Diffuse * cDiffuse;	
			
		#ifdef SPEC			
			flagColor.xyz += Specular * u_cSpecColor.xyz/*specIntensity*/;	
		#endif 
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
	
	return flagColor;
}
#endif

void main(PS_IN In,
#if DEFERREDSHADING != 0
out PS_OUT pout
#else
out float4 outColor : SV_TARGET 
#endif
) 
{
	float4 cDiffuse = GetDiffuse(In); 

#if DEFERREDSHADING != 0
	float4 cSpecColorPow = 0;
	#ifdef SPEC
		cSpecColorPow.xyz = u_cSpecColor.xyz * u_cSpecColor.w;
		cSpecColorPow.w = u_specPower;
	#endif	
	pout = GbufferPSout(cDiffuse,cSpecColorPow,In.viewNormal);
#else
	outColor = ForwardShading(cDiffuse,In);  
#endif	
}
