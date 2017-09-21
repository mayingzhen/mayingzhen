


cbuffer ShadowVS : register(b6)
{
	float4x4 g_matShadow;

#if SHADOW_BLUR == 2
	float4 g_vViewPosVecLS;
#endif
};

#if SHADOW_BLUR == 2
void GetRandDirTC(float fDistance,out float2 randDirTC)
{	
	randDirTC = g_vViewPosVecLS.xy + g_vViewPosVecLS.zw * fDistance;
}
#endif

void GetShadowPos(float3 iWordPos,out float4 shadowPos)
{
 	shadowPos = mul(float4(iWordPos,1.0), g_matShadow);
}

