#include"common.h"

Texture2D tDeviceDepthMapSampler;
SamplerState sDeviceDepthMapSampler;

void main(float4 oPos     : SV_POSITION,
        float2 iUV      : TEXCOORD0,
    
        out float4 oColor   :SV_TARGET)
{
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tDeviceDepthMapSampler.Sample(sDeviceDepthMapSampler, iUV).r;
	depth = g_vCameraNearFar.x / (q - depth);
	oColor = depth * g_vCameraNearFar.z;
}


