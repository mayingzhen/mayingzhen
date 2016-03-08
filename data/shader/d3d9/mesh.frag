
#include "common.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

uniform sampler2D u_texture;


#ifdef DIFFUSECOLOR   
uniform float4 u_cDiffuseColor; 
#endif

uniform float4 u_cSpecColor;
uniform float u_specPower;

// Varyings
struct PS_IN
{
   float2   v_texCoord : TEXCOORD0;

	float4 WorldPos : TEXCOORD1;
	float4 worldNormal :TEXCOORD2;

#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD3;	
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

   flagColor *= tex2D(u_texture, In.v_texCoord);

	return flagColor;
}

#ifndef DEFERREDSHADING 
float4 ForwardShading(float4 cDiffuse,PS_IN In)
{
	float4 flagColor = cDiffuse;
	
	In.worldNormal = normalize(In.worldNormal);

#ifdef LIGHT	
	float fNDotL = clamp(dot(In.worldNormal, g_vDirLight), 0, 1);
    float3 fDirLight = g_cDirLight * fNDotL;
    flagColor.rgb *= g_cSkyLight + fDirLight;
    
    #ifdef SPEC
		float3 refl = reflect(-g_vDirLight, In.worldNormal);	
		float cosVal = dot(normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz), refl);
		float3 spec = g_cDirLight* (pow(max(0,cosVal), u_specPower) * u_cSpecColor);
		flagColor.rgb += spec;
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
#ifdef DEFERREDSHADING 
out PS_OUT pout
#else
out float4 outColor : COLOR0 
#endif
) 
{
	float4 cDiffuse = GetDiffuse(In); 

#ifdef DEFERREDSHADING 
	pout = GbufferPSout(cDiffuse,u_cSpecColor,In.v_normalDepth);
#else
	outColor = ForwardShading(cDiffuse,In);  
#endif	
}
