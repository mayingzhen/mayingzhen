
TextureCube tEnv : register(t0);
SamplerState sEnv : register(s0);

void main(float3 iUV      : TEXCOORD0,
        out float4 oColor   :SV_TARGET)
{
	oColor = tEnv.SampleLevel(sEnv, iUV, 0);
}