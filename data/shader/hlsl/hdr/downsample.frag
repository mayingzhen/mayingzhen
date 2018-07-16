Texture2D g_TexSrc;
SamplerState g_SamplerSrc;


float4 main(float2 tex : TEXCOORD0) : SV_TARGET
{
	return g_TexSrc.Sample(g_SamplerSrc, tex);
}
