float frame_delta;

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

texture last_lum_tex;
sampler g_SamplerLastLum = sampler_state
{
	Texture = <last_lum_tex>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};


float CalcAdaptedLum(float adapted_lum, float current_lum)
{
	return adapted_lum + (current_lum - adapted_lum) * (1 - pow(0.98f, 50 * frame_delta));
}

float4 AdaptedLumPS() : COLOR
{
	float adapted_lum = tex2D(g_SamplerLastLum, float2(0.5f, 0.5f) ).r;
	float current_lum = exp( tex2D(g_SamplerSrc, float2(0.5f, 0.5f) ).r);

	return CalcAdaptedLum(adapted_lum, current_lum);
}
	
technique AdaptedLum
{
	pass P0
	{
		PixelShader = compile ps_2_0 AdaptedLumPS();
		
		CullMode = cw;
	}
}