
sampler2D g_SamplerSrc;


float4 main(float2 tex : TEXCOORD0) : COLOR
{
	return tex2D(g_SamplerSrc, tex);
}
