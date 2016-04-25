
// texture
Texture2D g_tShadowMap : register(t5);
SamplerState g_sShadowMap : register(s5);

Texture2D tRotSampler : register(t6);
SamplerState sRotSampler : register(s6);

#ifndef SHADOW_SAMPLES_NUM
#define SHADOW_SAMPLES_NUM 8
#endif

cbuffer ShadowVS : register(b6)
{
	float4x4 g_matShadow;
}

cbuffer ShadowPS : register(b6)
{
	float4 g_shadowMapTexelSize;
	float4 g_ShadowDepthFade;

	float4 irreg_kernel_2d[SHADOW_SAMPLES_NUM/2];
	float4 g_vViewPosVecLS;
	float2 kernelRadius;
}


float GetDirShadowFade(float inLight, float depth)
{
	return saturate(inLight + saturate((depth - g_ShadowDepthFade.z) * g_ShadowDepthFade.w));
}

float SamplerDepth(float4 vTexCoord)
{
#if USING_FLOATTEXTURE == 1
	return g_tShadowMap.Sample(g_sShadowMap, vTexCoord.xy / vTexCoord.w).r;
#else
	return DecodeFloatRGBA( g_tShadowMap.Sample(g_sShadowMap, vTexCoord.xy / vTexCoord.w) );
#endif
}



float ShadowDepthCompare(float4 vTexCoord)
{
#if USING_HW_PCF == 1
	return g_tShadowMap.SampleCmpLevelZero(g_sShadowMap, vTexCoord.xy, vTexCoord.z);
#else 
	float fDepth = SamplerDepth(vTexCoord);
	return fDepth > vTexCoord.z / vTexCoord.w;
#endif
}


float shadow_pcf(float4 shadowPos)
{
	float fShadow0 = ShadowDepthCompare(shadowPos);
	float fShadow1 = ShadowDepthCompare(float4(shadowPos.x + g_shadowMapTexelSize.y, shadowPos.yzw));
	float fShadow2 = ShadowDepthCompare(float4(shadowPos.x, shadowPos.y + g_shadowMapTexelSize.y, shadowPos.zw));
	float fShadow3 = ShadowDepthCompare(float4(shadowPos.xy + g_shadowMapTexelSize.yy, shadowPos.zw));

	return (fShadow0 + fShadow1 + fShadow2 + fShadow3) * 0.25f;
}





void GetRandDirTC(float fDistance,out float2 randDirTC)
{	
	randDirTC = g_vViewPosVecLS.xy + g_vViewPosVecLS.zw * fDistance;
}

void GetShadowPos(float3 iWordPos,out float4 shadowPos)
{
 	shadowPos = mul(float4(iWordPos,1.0), g_matShadow);
}


void DoubleSampleRotated(float4 shPos,float4 rotMatr,float4 kernel,out float2 result)
{
	float4 rotatedOff = rotMatr.xyzw * kernel.xxww + rotMatr.zwxy * kernel.yyzz;

	result.x = ShadowDepthCompare(shPos + rotatedOff.xyzw * float4(1,1,0,0) );
	result.y = ShadowDepthCompare(shPos + rotatedOff.zwxy * float4(1,1,0,0) );	
}


float shadow_jitterin(float4 shadowPos,float2 randDirTC)
{
	float shadowTest = 0;

	//float2 kernelRadius = float2(4.0f,4.0f);//float2(1.0f,1.0f) * (SHADOW_SAMPLES_NUM / 8);
	float2 rotScale = kernelRadius.y * 15.0f;

	float2 rotSample = 2.0f * tRotSampler.Sample(sRotSampler, randDirTC.xy * rotScale.xy).xy - 1.f;
	rotSample.xy = normalize(rotSample.xy);
	rotSample.xy *= kernelRadius.xy * g_shadowMapTexelSize.yy;
	float4 rot = float4(rotSample.x,-rotSample.y,rotSample.y,rotSample.x); // 2x2µÄÐý×ª¾ØÕó

#if SHADOW_SAMPLES_NUM == 4
	float fInvSamplNum = 1.0 / 4.0;
	float4 DepthTest = 0;
	DoubleSampleRotated(shadowPos,rot,irreg_kernel_2d[0],DepthTest.xy);
	DoubleSampleRotated(shadowPos,rot,irreg_kernel_2d[1],DepthTest.zw);
	shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
#else	
	float fInvSamplNum = 1.0 / SHADOW_SAMPLES_NUM;
	float kernelSize = SHADOW_SAMPLES_NUM / 2;
	for(int i = 0; i < kernelSize; i += 2)
	{
		float4 DepthTest = 0;
		DoubleSampleRotated(shadowPos,rot,irreg_kernel_2d[i+0],DepthTest.xy);
		DoubleSampleRotated(shadowPos,rot,irreg_kernel_2d[i+1],DepthTest.zw);
		shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
	}
#endif


	return shadowTest;
}

float DoShadowMapping(float4 vShadowPos,float2 vRandDirTC,float fDistance)
{
	float fShadow = 1.0f;

#if SHADOW_BLUR == 2
	fShadow = shadow_jitterin(vShadowPos,vRandDirTC);
#elif SHADOW_BLUR == 1
	fShadow = shadow_pcf(vShadowPos);
#else
	fShadow = ShadowDepthCompare(vShadowPos);
#endif
	
	fShadow = fShadow + 0.7;

#ifdef SHADOW_FADE
	fShadow = GetDirShadowFade(fShadow,fDistance);
#else
	fShadow = saturate(fShadow);
#endif


	return fShadow;
}	






