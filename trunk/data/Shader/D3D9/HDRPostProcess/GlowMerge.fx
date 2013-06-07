
texture glow_tex_0;
sampler g_SamplerGlowTex0 = sampler_state
{
	Texture = <glow_tex_0>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture glow_tex_1;
sampler g_SamplerGlowTex1 = sampler_state
{
	Texture = <glow_tex_1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture glow_tex_2;
sampler g_SamplerGlowTex2 = sampler_state
{
	Texture = <glow_tex_2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};


float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1;
	tex += 0.5;
	return tex;
}

void PostProcessVS(float4 pos : POSITION,
					out float2 oTex : TEXCOORD0,
					out float4 oPos : POSITION)
{
	oTex = TexCoordFromPos(pos);
	oPos = pos;
}


float4 GlowMergerPS(float2 tex : TEXCOORD0) : COLOR
{
	float4 clr0 = tex2D(g_SamplerGlowTex0, tex);
	float4 clr1 = tex2D(g_SamplerGlowTex1, tex);
	float4 clr2 = tex2D(g_SamplerGlowTex2, tex);
	
	return clr0 * 2.0f + clr1 * 1.15f + clr2 * 0.45f;
}

technique GlowMerge
{
	pass P0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader = compile ps_3_0 GlowMergerPS();
	
		CullMode = cw;
	}
}