Texture2D g_TexGlowTex0;
SamplerState g_SamplerGlowTex0;

Texture2D g_TexGlowTex1;
SamplerState g_SamplerGlowTex1;

Texture2D g_TexGlowTex2;
SamplerState g_SamplerGlowTex2;


float4 main(float2 tex : TEXCOORD0) : SV_TARGET
{
	float4 clr0 = g_TexGlowTex0.Sample(g_SamplerGlowTex0, tex);
	float4 clr1 = g_TexGlowTex1.Sample(g_SamplerGlowTex1, tex);
	float4 clr2 = g_TexGlowTex2.Sample(g_SamplerGlowTex2, tex);
	
	return clr0 * 2.0f + clr1 * 1.15f + clr2 * 0.45f;
}

