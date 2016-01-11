
float4x4 matFrustum;

void vs(float3 iPos			: POSITION0,
		out float4 oPos     : POSITION0) 
{

	float4 vPos = float4(iPos,1.0);

	vPos = mul(vPos, matFrustum);

	//normalization
	vPos.xyz /= vPos.w;
	vPos.w = 1.0f;

	//final ViewProj transformation
	oPos = mul(vPos,g_matViewProj); 
}
