
cbuffer ObjectPS : register(b7)
{
	float4 vStoWBasisX;
	float4 vStoWBasisY;
	float4 vStoWBasisZ;
	float4 vStoCamPos;
};


Texture2D tDepthMapSampler;
SamplerState sDepthMapSampler;

Texture2D tDeviceDepthMapSampler;
SamplerState sDeviceDepthMapSampler;


float GetLinearDepth(float2 tc)
{
#if USING_LINEARDEPTH == 1
	float depth = tDepthMapSampler.Sample(sDepthMapSampler, tc).r;
	depth *= g_vCameraNearFar.y;
	return depth;
#else 
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tDeviceDepthMapSampler.Sample(sDeviceDepthMapSampler, tc).r;
	depth = g_vCameraNearFar.x / (q - depth);
	return depth;
#endif
}

float4 CalcHomogeneousPosDepth(float2 tc,float2 WPos)
{
	float fLinearDepth = GetLinearDepth(tc);

	float3 vViewDir = vStoWBasisZ.xyz + vStoWBasisX.xyz * WPos.x + vStoWBasisY.xyz * WPos.y; 
	float3 vPos = vViewDir * fLinearDepth * g_vCameraNearFar.z  + vStoCamPos.xyz;

	float4 HPos = float4(vPos,fLinearDepth);
	return HPos;
}
