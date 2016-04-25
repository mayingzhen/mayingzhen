#include "common.h"

#if USING_SHADOW != 0
#include "shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include "gbuffer.h"
#endif

cbuffer ObjectPS : register(b5)
{
	uniform float4 u_cSpecColor;

	uniform float2 uBlendingOffset;
}

Texture2D tBlendingMap : register(t2);
SamplerState sBlendingMap : register(s2);

Texture2D tDetailMap0 : register(t0);
SamplerState sDetailMap0 : register(s0);

Texture2D tDetailMap1 : register(t1);
SamplerState sDetailMap1 : register(s1);

Texture2D tBumpMap0 : register(t3);
SamplerState sBumpMap0 : register(s3);

Texture2D tBumpMap1 : register(t4);
SamplerState sBumpMap1 : register(s4);

struct VS_OUTPUT
{
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;
	float4 Color	: TEXCOORD2;
	float4 WorldPos : TEXCOORD3;
#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD4;	
#else 
#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
#if SHADOW_BLUR == 2
	float2 oRandDirTC : TEXCOORD4;
#endif	
	float4 oShadowPos : TEXCOORD5;
#endif
#endif	
};


float4 GetDiffuse(VS_OUTPUT In)
{
	float4 oColor = float4(1,1,1,1);
	
	float4 cBlend = tBlendingMap.Sample(sBlendingMap, In.UV + uBlendingOffset);
		
    // Ï¸½ÚÍ¼
#if LAYER==1
 	float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
	oColor = cDetailMap0;
#elif LAYER==2
    float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
    float4 cDetailMap1 = tDetailMap1.Sample(sDetailMap1, In.DetailUV.zw);
    oColor = cDetailMap0 * cBlend.a + cDetailMap1 * (1.0 - cBlend.a);
#endif

	oColor.a = In.Color.a;		

	return oColor;
}

#ifndef DEFERREDSHADING 
float4 ForwardShading(float4 cDiffuse,VS_OUTPUT In)
{
	float4 flagColor = cDiffuse;

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float4 ShadowPos = In.oShadowPos;
	float2 RandDirTC = 0;
#if SHADOW_BLUR == 2
	RandDirTC = In.oRandDirTC;
#endif
	flagColor.rgb *= DoShadowMapping(ShadowPos,RandDirTC,In.WorldPos.w);		
#endif
	
	return flagColor;
}
#endif

void main(VS_OUTPUT In,
#if defined(DEFERREDSHADING)
out PS_OUT pout
#else
out float4 outColor : SV_TARGET 
#endif
)
{
	float4 cDiffuse = GetDiffuse(In);

#if defined(DEFERREDSHADING) 
	pout = GbufferPSout(cDiffuse,u_cSpecColor,In.v_normalDepth);
#else
	outColor = ForwardShading(cDiffuse,In);
#endif	
}



