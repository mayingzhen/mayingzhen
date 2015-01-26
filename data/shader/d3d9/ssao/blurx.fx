static const int kernel_radius = 8;
float2 src_tex_size;
float init_g;
float blur_factor;
float sharpness_factor;

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

float BilateralDistribution(float r, float d)
{
	return init_g * exp(-r * r * blur_factor - d * d * sharpness_factor);
}

float4 CalcBlur(float2 tc0, bool x_dir)
{
	float color = 0;

	float center_d = tex2D(g_SamplerSrc, tc0).y;

	float total_weight = 0;
	for (int i = -kernel_radius; i <= kernel_radius; ++ i)
	{
		float2 offset = float2(i * src_tex_size.y, 0);
		float2 tc = tc0 + (x_dir ? offset.xy : offset.yx);
		float2 src = tex2D(g_SamplerSrc, tc).rg;
		float ddiff = src.y - center_d;
		float weight = BilateralDistribution(i, ddiff);
		color += src.x * weight;
		total_weight += weight;
	}

	return float4(color / total_weight, center_d, 0, 0);
}

float4 BlurXPS(float2 tc0 : TEXCOORD0) : COLOR
{
	return CalcBlur(tc0, true);
}


technique BlurX
{
	pass p0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader =  compile ps_3_0 BlurXPS();
		
		CullMode = cw;
	}	
}