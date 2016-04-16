
float4 vStoWBasisX;
float4 vStoWBasisY;
float4 vStoWBasisZ;
float4 vStoCamPos;



sampler2D tDepthMapSampler;

sampler2D tDeviceDepthMapSampler;



float GetLinearDepth(float2 tc)
{
#if USING_LINEARDEPTH == 1
	float depth = tex2D(tDepthMapSampler, tc).r;
	depth *= g_vCameraNearFar.y;
	return depth;
#else 
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tex2D(tDeviceDepthMapSampler, tc).r;
	depth = g_vCameraNearFar.x / (q - depth);
	return depth;
#endif
}

float4 CalcHomogeneousPos(float2 tc,float2 WPos)
{
	float fLinearDepth = GetLinearDepth(tc);

	float3 vViewDir = vStoWBasisZ.xyz + vStoWBasisX.xyz * WPos.x + vStoWBasisY.xyz * WPos.y; 
	float3 vPos = vViewDir * fLinearDepth * g_vCameraNearFar.z  + vStoCamPos.xyz;

	float4 HPos = float4(vPos,fLinearDepth);
	return HPos;
}
