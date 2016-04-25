#include"common.h"

Texture2D tDiff : register(t0);
SamplerState sDiff : register( s0 );

struct PS_IN
{
	float2 oUV	: TEXCOORD0;
		
#if USING_HW_PCF == 0
	float4 oDepth : TEXCOORD1;
#endif	
};


float4 OutPutDepth(float fDepth)
{
	fDepth += 0.0010f;

#if USING_FLOATTEXTURE == 1
	return fDepth;
#else
	return EncodeFloatRGBA(fDepth);
#endif	
}

float4 main(PS_IN In) : SV_TARGET
{
	float4 oColor = 0;
#ifdef ALPHATEST
    oColor.a = tDiff.Sample(sDiff, In.oUV).a;
    clip(oColor.a - 0.5);
#endif

#if USING_HW_PCF == 0
	oColor = OutPutDepth(In.oDepth.z / In.oDepth.w);
#else
	oColor = 1.0;
#endif

	return oColor;
}


