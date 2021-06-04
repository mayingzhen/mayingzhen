#include"common.h"

Texture2D tDeviceDepthMapSampler;
SamplerState sDeviceDepthMapSampler;

struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
   float4 pos : SV_POSITION;
};

void vs_main(
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
	out float2 oTc : TEXCOORD0,
	out float4 oPos : SV_POSITION)
{
	oPos = float4(pos.x, pos.y, 0.0, 1);
	oTc = iUV;
}


void ps_main(float2 iUV      : TEXCOORD0,
        out float4 oColor   :SV_TARGET)
{
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tDeviceDepthMapSampler.Sample(sDeviceDepthMapSampler, iUV).r;
	depth = g_vCameraNearFar.x / (q - depth);
	oColor = depth * g_vCameraNearFar.z;
}


