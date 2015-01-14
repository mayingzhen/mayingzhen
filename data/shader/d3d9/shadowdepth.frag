#include"common.h"

struct PS_IN
{
	float4 oPos : POSITION;
	
	float2 oUV	: TEXCOORD0;
		
#ifndef HWPCF
	float4 oDepth : TEXCOORD1;
#endif	
};

float4 OutPutDepth(float fDepth)
{
	fDepth += 0.00015f;

#if USING_32F == 1
	return fDepth;
#else
	return EncodeFloatRGBA(fDepth);
#endif	
}

float4 main(PS_IN In) : COLOR0
{
	float4 oColor = 0;
#ifdef ALPHATEST
    oColor.a = tex2D(tDiff, In.oUV).a;
    clip(oColor.a - 0.5);
#endif

#if USING_HW_PCF == 0
	oColor = OutPutDepth(In.oDepth.z / In.oDepth.w);
#else
	oColor = 1.0;
#endif

	return oColor;
}


