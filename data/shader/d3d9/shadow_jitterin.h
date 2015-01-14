sampler2D sRotSampler;


texture tTextureRotSampler;
sampler tRotSampler
<
string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
string TextureAddressingMode = "TAM_WRAP,TAM_WRAP";
> = sampler_state
{
	Texture = (tTextureRotSampler);
};



#ifndef SHADOW_SAMPLES_NUM
#define SHADOW_SAMPLES_NUM 8
#endif

float4 irreg_kernel_2d[SHADOW_SAMPLES_NUM/2];
float4 g_vViewPosVecLS[g_iNumSplits];



void DoubleSampleRotated(sampler2D depthMap,float4 shPos,float4 rotMatr,float4 kernel,out float2 result)
{
	float4 rotatedOff = rotMatr.xyzw * kernel.xxww + rotMatr.zwxy * kernel.yyzz;

	result.x = ShadowDepthCompare( depthMap, shPos + rotatedOff.xyzw * float4(1,1,0,0) );
	result.y = ShadowDepthCompare( depthMap, shPos + rotatedOff.zwxy * float4(1,1,0,0) );	
}


float2 GetRandDirTC(int index,float fDistance)
{
	float SceneDepth = fDistance * g_vCameraNearFar.z;
	float2 randDirTC = g_vViewPosVecLS[index].xy + g_vViewPosVecLS[index].zw * SceneDepth;

	return randDirTC;
}

float shadow_jitterin(sampler2D shadowMap,float4 shadowPos,float2 randDirTC)
{
	float shadowTest = 0;

	float2 kernelRadius = float2(2.0f,2.0f);//float2(1.0f,1.0f) * (SHADOW_SAMPLES_NUM / 8);
	float2 rotScale = kernelRadius.y * 15.0f;

	float2 rotSample = 2.0f * tex2D(tRotSampler, randDirTC.xy * rotScale.xy).xy - 1.f;
	rotSample.xy = normalize(rotSample.xy);
	rotSample.xy *= kernelRadius.xy * g_shadowMapTexelSize.yy;
	float4 rot = float4(rotSample.x,-rotSample.y,rotSample.y,rotSample.x); // 2x2µÄÐý×ª¾ØÕó

#if SHADOW_SAMPLES_NUM == 4
	float fInvSamplNum = 1.0 / 4.0;
	float4 DepthTest = 0;
	DoubleSampleRotated(shadowMap,shadowPos,rot,irreg_kernel_2d[0],DepthTest.xy);
	DoubleSampleRotated(shadowMap,shadowPos,rot,irreg_kernel_2d[1],DepthTest.zw);
	shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
#else	
	float fInvSamplNum = 1.0 / SHADOW_SAMPLES_NUM;
	float kernelSize = SHADOW_SAMPLES_NUM / 2;
	for(int i = 0; i < kernelSize; i += 2)
	{
		float4 DepthTest = 0;
		DoubleSampleRotated(shadowMap,shadowPos,rot,irreg_kernel_2d[i+0],DepthTest.xy);
		DoubleSampleRotated(shadowMap,shadowPos,rot,irreg_kernel_2d[i+1],DepthTest.zw);
		shadowTest += dot(DepthTest,fInvSamplNum.xxxx);
	}
#endif


	return shadowTest;
}
