
// texture
Texture2D g_tShadowMap : register(t5);
SamplerComparisonState g_sShadowMap : register(s5);

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
	float g_fShadowExt;

	float4 irreg_kernel_2d[SHADOW_SAMPLES_NUM/2];
	float4 g_vViewPosVecLS;
	float2 kernelRadius;
}


float GetDirShadowFade(float inLight, float depth)
{
	return saturate(inLight + saturate((depth - g_ShadowDepthFade.z) * g_ShadowDepthFade.w));
}

float ShadowDepthCompare(Texture2D tSam, SamplerComparisonState sSam, float4 vTexCoord)
{
	return tSam.SampleCmpLevelZero(sSam, vTexCoord.xy, vTexCoord.z);
}


float shadow_pcf(Texture2D tSam, SamplerComparisonState sSam, float4 shadowPos,float4 shadowMapTexelSize)
{
	float fShadow0 = ShadowDepthCompare(tSam, sSam, shadowPos);
	float fShadow1 = ShadowDepthCompare(tSam, sSam, float4(shadowPos.x + shadowMapTexelSize.y, shadowPos.yzw));
	float fShadow2 = ShadowDepthCompare(tSam, sSam, float4(shadowPos.x, shadowPos.y + shadowMapTexelSize.y, shadowPos.zw));
	float fShadow3 = ShadowDepthCompare(tSam, sSam, float4(shadowPos.xy + shadowMapTexelSize.yy, shadowPos.zw));

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


void DoubleSampleRotated(Texture2D tDepthMap, SamplerComparisonState sDepthMap, float4 shPos,float4 rotMatr,float4 kernel,out float2 result)
{
	float4 rotatedOff = rotMatr.xyzw * kernel.xxww + rotMatr.zwxy * kernel.yyzz;

	result.x = ShadowDepthCompare( tDepthMap, sDepthMap, shPos + rotatedOff.xyzw * float4(1,1,0,0) );
	result.y = ShadowDepthCompare( tDepthMap, sDepthMap, shPos + rotatedOff.zwxy * float4(1,1,0,0) );	
}


float shadow_jitterin(Texture2D tShadowMap, SamplerComparisonState sShadowMap, float4 shadowPos,float2 randDirTC,float4 shadowMapTexelSize)
{
	float shadowTest = 0;

	//float2 kernelRadius = float2(4.0f,4.0f);//float2(1.0f,1.0f) * (SHADOW_SAMPLES_NUM / 8);
	float2 rotScale = kernelRadius.y * 15.0f;

	float2 rotSample = 2.0f * tRotSampler.Sample(sRotSampler, randDirTC.xy * rotScale.xy).xy - 1.f;
	rotSample.xy = normalize(rotSample.xy);
	rotSample.xy *= kernelRadius.xy * shadowMapTexelSize.yy;
	float4 rot = float4(rotSample.x,-rotSample.y,rotSample.y,rotSample.x); // 2x2µÄÐý×ª¾ØÕó

#if SHADOW_SAMPLES_NUM == 4
	float fInvSamplNum = 1.0 / 4.0;
	float4 DepthTest = 0;
	DoubleSampleRotated(tShadowMap, sShadowMap,shadowPos,rot,irreg_kernel_2d[0],DepthTest.xy);
	DoubleSampleRotated(tShadowMap, sShadowMap,shadowPos,rot,irreg_kernel_2d[1],DepthTest.zw);
	shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
#else	
	float fInvSamplNum = 1.0 / SHADOW_SAMPLES_NUM;
	float kernelSize = SHADOW_SAMPLES_NUM / 2;
	for(int i = 0; i < kernelSize; i += 2)
	{
		float4 DepthTest = 0;
		DoubleSampleRotated(tShadowMap, sShadowMap,shadowPos,rot,irreg_kernel_2d[i+0],DepthTest.xy);
		DoubleSampleRotated(tShadowMap, sShadowMap,shadowPos,rot,irreg_kernel_2d[i+1],DepthTest.zw);
		shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
	}
#endif


	return shadowTest;
}

float DoShadowMappingSampler(Texture2D tShadowMap,SamplerComparisonState sShadowMap,float4 vShadowPos,float2 vRandDirTC,float fDistance,float4 shadowMapTexelSize)
{
	float fShadow = 1.0f;

#if SHADOW_BLUR == 2
	fShadow = shadow_jitterin(tShadowMap,sShadowMap,vShadowPos,vRandDirTC,shadowMapTexelSize);
#elif SHADOW_BLUR == 1
	fShadow = shadow_pcf(tShadowMap,sShadowMap,vShadowPos,shadowMapTexelSize);
#else
	fShadow = ShadowDepthCompare(tShadowMap,sShadowMap, vShadowPos);
#endif

	fShadow = fShadow + g_fShadowExt;;

#ifdef SHADOW_FADE
	fShadow = GetDirShadowFade(fShadow,fDistance);
#else
	fShadow = saturate(fShadow);
#endif


	return fShadow;
}


float DoShadowMapping(float4 vShadowPos,float2 vRandDirTC,float fDistance)
{
	return DoShadowMappingSampler(g_tShadowMap,g_sShadowMap,vShadowPos,vRandDirTC,fDistance,g_shadowMapTexelSize);
}	


