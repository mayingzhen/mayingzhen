#include "../common.h"

float AdaptationRate;

// Varyings
struct VS_OUT
{
	float4 oPos : SV_POSITION;

	float2 v_texCoord0 : TEXCOORD0;
};

VS_OUT vs_main(float2 iPos     : POSITION0,
			float2 iUV		: TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;

	Out.oPos = float4(iPos.x, iPos.y, 1.0, 1.0);
	Out.v_texCoord0.xy = iUV.xy;

	return Out;
}


Texture2D tSrcColor;
SamplerState sSrcColor;

Texture2D g_Texturelast;
SamplerState g_Samplerlast;

float LumLog(float3 color)
{
	const float3 RGB_TO_LUM = float3(0.299, 0.587, 0.114);
	float lum = dot(color, RGB_TO_LUM);
	lum = max(lum, 0.0001);
	return log( lum );
}

float4 log_main(VS_OUT vsIn) : SV_TARGET
{
	float2 mainTexSize;
	tSrcColor.GetDimensions(mainTexSize.x, mainTexSize.y);
	float2 texelSize = 1.0f / mainTexSize;
	float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0);

	float logLum = 0;
	logLum += LumLog( tSrcColor.Sample(sSrcColor, vsIn.v_texCoord0.xy + d.xy).rgb );
	logLum += LumLog( tSrcColor.Sample(sSrcColor, vsIn.v_texCoord0.xy + d.zy).rgb );
	logLum += LumLog( tSrcColor.Sample(sSrcColor, vsIn.v_texCoord0.xy + d.xw).rgb );
	logLum += LumLog( tSrcColor.Sample(sSrcColor, vsIn.v_texCoord0.xy + d.zw).rgb );
	logLum /= 4.0;
	
	return float4(logLum, logLum, logLum, 1.0f);
}	

// Standard box filtering
float4 DownsampleBox4Tap(Texture2D tex, SamplerState samplerTex, float2 uv, float2 texelSize)
{
    float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0);

    float4 s;
    s =  tex.Sample(samplerTex, uv + d.xy);
    s += tex.Sample(samplerTex, uv + d.zy);
    s += tex.Sample(samplerTex, uv + d.xw);
    s += tex.Sample(samplerTex, uv + d.zw);

    return s * (1.0 / 4.0);
}
	
float4 iter_main(VS_OUT vsIn) : SV_TARGET
{
    float2 mainTexSize;
	tSrcColor.GetDimensions(mainTexSize.x, mainTexSize.y);

	float4 color = DownsampleBox4Tap(tSrcColor, sSrcColor, vsIn.v_texCoord0, 1.0f / mainTexSize);
	
	return color;
}
	


float4 final_main(VS_OUT vsIn) : SV_TARGET
{
    float2 mainTexSize;
	tSrcColor.GetDimensions(mainTexSize.x, mainTexSize.y);

	float s = DownsampleBox4Tap(tSrcColor, sSrcColor, vsIn.v_texCoord0, 1.0f / mainTexSize).r;
	
	s = exp(s);

	//Adapt
	//float lastLum = g_Texturelast.Sample(g_Samplerlast, float2(0.5, 0.5)).r;
	//s = lastLum + (s - lastLum) * (1.0 - exp(-g_fTime * AdaptationRate));
	
   
    return float4(s, s, s, 1.0f);
}	
