
cbuffer ObjectVS : register(b5)
{
	float4x4 inv_mvp;
};

void vs_main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : SV_POSITION,
        out float3 oUv      : TEXCOORD0)
{
    oPos = float4(iPos.x, iPos.y, 1, 1);
	oUv = mul(oPos, inv_mvp).xyz;	
}


TextureCube tEnv : register(t0);
SamplerState sEnv : register(s0);

void ps_main(float3 iUV      : TEXCOORD0,
        out float4 oColor   :SV_TARGET)
{
	oColor = tEnv.SampleLevel(sEnv, iUV, 0);
}