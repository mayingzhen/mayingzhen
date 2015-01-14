
// texture
sampler g_tShadowMap;


#ifndef g_iNumSplits
#define g_iNumSplits 1
#endif

uniform float4x4 g_matShadow[g_iNumSplits];
uniform float4 g_fSplitPlane;
uniform float4 g_shadowMapTexelSize;
uniform float4 g_ShadowDepthFade;



float GetDirShadowFade(float inLight, float depth)
{
	return saturate(inLight + saturate((depth - g_ShadowDepthFade.z) * g_ShadowDepthFade.w));
}

float SamplerDepth(sampler2D sam, float4 vTexCoord)
{
#if USING_32F == 1
	return tex2Dproj(sam, vTexCoord).r;
#else
	return DecodeFloatRGBA( tex2Dproj(sam, vTexCoord) );
#endif
}


float ShadowDepthCompare(sampler2D sam, float4 vTexCoord)
{
#if USING_HW_PCF == 1
	return tex2Dproj(sam, vTexCoord).r;
#else 
	float fDepth = SamplerDepth(sam,vTexCoord);
	return fDepth > vTexCoord.z / vTexCoord.w;
#endif
}


float shadow_pcf(sampler2D sam, float4 shadowPos)
{
	float fShadow0 = ShadowDepthCompare(sam, shadowPos);
	float fShadow1 = ShadowDepthCompare(sam, float4(shadowPos.x + g_shadowMapTexelSize.y, shadowPos.yzw));
	float fShadow2 = ShadowDepthCompare(sam, float4(shadowPos.x, shadowPos.y + g_shadowMapTexelSize.y, shadowPos.zw));
	float fShadow3 = ShadowDepthCompare(sam, float4(shadowPos.xy + g_shadowMapTexelSize.yy, shadowPos.zw));

	return (fShadow0 + fShadow1 + fShadow2 + fShadow3) * 0.25f;
}

#include "shadow_jitterin.h"



void GetRandDirTC(float fDistance,out float2 randDirTC)
{	
	randDirTC = float2(0,0);

#if g_iNumSplits > 0
	if (fDistance < g_fSplitPlane.x)
	{
		randDirTC = GetRandDirTC(0,fDistance);
	}
#endif
#if g_iNumSplits > 1
	else if (fDistance < g_fSplitPlane.y)
	{
		randDirTC = GetRandDirTC(1,fDistance);
	}
#endif
#if g_iNumSplits > 2
	else if (fDistance < g_fSplitPlane.z)
	{
		randDirTC = GetRandDirTC(2,fDistance);
	}
#endif
#if g_iNumSplits > 3
	else if (fDistance < g_fSplitPlane.w)
	{
		randDirTC = GetRandDirTC(3,fDistance);
	}
#endif

}


void GetShadowPosArray(float3 iWordPos,out float4 shadowPos[g_iNumSplits])
{
#if g_iNumSplits > 0
	shadowPos[0] = mul(float4(iWordPos,1.0), g_matShadow[0]);
#endif
#if g_iNumSplits >1
	shadowPos[1] = mul(float4(iWordPos,1.0), g_matShadow[1]); 
#endif
#if g_iNumSplits > 2
	shadowPos[2] = mul(float4(iWordPos,1.0), g_matShadow[2]); 
#endif
#if g_iNumSplits > 3
	shadowPos[3] = mul(float4(iWordPos,1.0), g_matShadow[3]); 
#endif
}


void GetShadowPosForamArray(float fDistance,float4 shadowPosArry[g_iNumSplits],out float4 shadowPos)
{
	shadowPos = float4(0,0,0,1.0);
#if g_iNumSplits > 0
	if (fDistance < g_fSplitPlane.x)
	{
		shadowPos = shadowPosArry[0];

#if g_iNumSplits == 3 || g_iNumSplits == 4 	
		shadowPos.x = clamp(shadowPos.x,0,0.5f);
		shadowPos.y = clamp(shadowPos.y,0,0.5f);
#elif g_iNumSplits == 2
		shadowPos.x = clamp(shadowPos.x,0,0.5f);
#endif    
	}
#endif
#if g_iNumSplits > 1
	else if (fDistance < g_fSplitPlane.y)
	{
		shadowPos = shadowPosArry[1];

#if g_iNumSplits == 3 || g_iNumSplits == 4 	
		shadowPos.x = clamp(shadowPos.x,0.5f,1.0f);
		shadowPos.y = clamp(shadowPos.y,0,0.5f);
#elif g_iNumSplits == 2
		shadowPos.x = clamp(shadowPos.x,0.5,1.0f);		
#endif 
	}
#endif
#if g_iNumSplits > 2
	else if (fDistance < g_fSplitPlane.z)
	{
		shadowPos = shadowPosArry[2];
		shadowPos.x = clamp(shadowPos.x,0,0.5f);
		shadowPos.y = clamp(shadowPos.y,0.5f,1.0f);
	}
#endif
#if g_iNumSplits > 3
	else if (fDistance < g_fSplitPlane.w)
	{
		shadowPos = shadowPosArry[3];
		shadowPos.x = clamp(shadowPos.x,0.5f,1.0f);
		shadowPos.y = clamp(shadowPos.y,0.5f,1.0f);
	}
#endif
}

void GetShadowPos(float3 iWordPos,float fDistance,out float4 shadowPos[g_iNumSplits],out float2 randDirTC)
{
	randDirTC = float2(0,0);

	GetShadowPosArray(iWordPos,shadowPos);

	GetRandDirTC(fDistance,randDirTC);	
}


float DoShadowMapping(float4 vShadowPos[g_iNumSplits],float2 vRandDirTC,float fDistance)
{
	float fShadow = 1.0f;

	float4 shadowPos = 0;
	GetShadowPosForamArray(fDistance,vShadowPos,shadowPos);

#if SHADOW_BLUR == 2
	fShadow = shadow_jitterin(g_tShadowMap,shadowPos,vRandDirTC);
#elif SHADOW_BLUR == 1
	fShadow = shadow_pcf(g_tShadowMap,shadowPos);
#else
	fShadow = ShadowDepthCompare(g_tShadowMap, shadowPos);
#endif
	
	fShadow = fShadow + 0.333f;
	fShadow = GetDirShadowFade(fShadow,fDistance);


	return fShadow;
}	




