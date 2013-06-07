
texture g_TextureSrc;
sampler g_SamplerSrc = sampler_state
{
	Texture = <g_TextureSrc>;
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


float4 DownsamplePS(float2 tex : TEXCOORD0) : COLOR
{
	return tex2D(g_SamplerSrc, tex);
}

technique Downsample
{
	pass P0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader = compile ps_3_0 DownsamplePS();
		
		CullMode = cw;
	}
}