#include "../common.h"

Texture2D g_TextureSrc;
SamplerState g_SamplerSrc;

Texture2D g_Texturelast;
SamplerState g_Samplerlast;

cbuffer ObjectPS : register(b5)
{
	float4 tex_coord_offset[2];
	float AdaptationRate;
}

float4 main(float2 itex : TEXCOORD0) : SV_TARGET
{
	float4 tex[2];
	
	tex[0] = itex.xyxy + tex_coord_offset[0];
	tex[1] = itex.xyxy + tex_coord_offset[1];

	float s = 0;
	for (int i = 0; i < 2; ++ i)
	{
		s += g_TextureSrc.Sample(g_SamplerSrc, tex[i].xy).r;
		s += g_TextureSrc.Sample(g_SamplerSrc, tex[i].zw).r;
	}

	s /= 4.0;
	
	s = exp(s);

	//Adapt
	float lastLum = g_Texturelast.Sample(g_Samplerlast, float2(0.5, 0.5)).r;
	s = lastLum + (s - lastLum) * (1.0 - exp(-g_fTime * AdaptationRate));
	
   
    return float4(s, s, s, 1.0f);
}	

