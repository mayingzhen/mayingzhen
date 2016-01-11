// #define DEFERREDSHADING
// #define USING_SHADOW 1 

#include "common.h"

#if USING_SHADOW != 0
#include "shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include "gbuffer.h"
#endif

uniform float4 u_cSpecColor;

sampler tBlendingMap;

sampler tDetailMap0;
sampler tDetailMap1;

sampler tBumpMap0;
sampler tBumpMap1;

uniform float2 uBlendingOffset;

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;
	float4 Color	: TEXCOORD2;
	float4 WorldPos : TEXCOORD3;
#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD4;	
#else 
#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float2 oRandDirTC : TEXCOORD4;
	float4 oShadowPos : TEXCOORD5;
#endif
#endif	
};


float4 GetDiffuse(VS_OUTPUT In)
{
	float4 oColor = float4(1,1,1,1);
	
	float4 cBlend = tex2D(tBlendingMap, In.UV + uBlendingOffset);
		
    // Ï¸½ÚÍ¼
#if LAYER==1
 	float4 cDetailMap0 = tex2D(tDetailMap0, In.DetailUV.xy);
	oColor = cDetailMap0;
#elif LAYER==2
    float4 cDetailMap0 = tex2D(tDetailMap0, In.DetailUV.xy);
    float4 cDetailMap1 = tex2D(tDetailMap1, In.DetailUV.zw);
    oColor = cDetailMap0 * cBlend.a + cDetailMap1 * (1.0 - cBlend.a);
#endif

#ifdef BOREDER
	oColor.a = In.Color.a;		
#endif

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
#if defined(DEFERREDSHADING)/* && !defined(BOREDER)*/
out PS_OUT pout
#else
out float4 outColor : COLOR0 
#endif
)
{
	float4 cDiffuse = GetDiffuse(In);

#if defined(DEFERREDSHADING) /*&& !defined(BOREDER) */
	pout = GbufferPSout(cDiffuse,u_cSpecColor,In.v_normalDepth);
#else
	outColor = ForwardShading(cDiffuse,In);
#endif	
}



