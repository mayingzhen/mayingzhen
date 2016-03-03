
#include "common.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

#ifdef DIFFUSE   
uniform sampler2D u_texture;
#endif


#ifdef DIFFUSECOLOR   
uniform float4 u_cDiffuseColor; 
#endif

uniform float4 u_cSpecColor;

// Varyings
struct PS_IN
{
#ifdef DIFFUSE
   float2   v_texCoord : TEXCOORD0;
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

#ifdef DIFFUSE
   flagColor *= tex2D(u_texture, In.v_texCoord);
#endif 

	return flagColor;
}

#ifndef DEFERREDSHADING 
float4 ForwardShading(float4 cDiffuse,PS_IN In)
{
	float4 flagColor = cDiffuse;

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
