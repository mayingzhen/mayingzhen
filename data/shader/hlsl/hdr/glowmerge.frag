
sampler2D g_SamplerGlowTex0;

sampler2D g_SamplerGlowTex1;

sampler2D g_SamplerGlowTex2;


float4 main(float2 tex : TEXCOORD0) : COLOR
{
	float4 clr0 = tex2D(g_SamplerGlowTex0, tex);
	float4 clr1 = tex2D(g_SamplerGlowTex1, tex);
	float4 clr2 = tex2D(g_SamplerGlowTex2, tex);
	
	return clr0 * 2.0f + clr1 * 1.15f + clr2 * 0.45f;
}

