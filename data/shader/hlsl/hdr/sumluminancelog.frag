float4 tex_coord_offset[2];

Texture2D g_TexSrc;
SamplerState g_SamplerSrc;

float LumLog(float3 color)
{
	const float3 RGB_TO_LUM = float3(0.299, 0.587, 0.114);
	float lum = dot(color, RGB_TO_LUM);
	lum = max(lum, 0.0001);
	return log( lum );
}

float4 main(float2 itex : TEXCOORD0) : SV_TARGET
{
	float4 tex[2] ;
	
	tex[0] = itex.xyxy + tex_coord_offset[0];
	tex[1] = itex.xyxy + tex_coord_offset[1];

	float logLum = 0;
	for (int i = 0; i < 2; ++ i)
	{
		logLum += LumLog( g_TexSrc.Sample(g_SamplerSrc, tex[i].xy).rgb );
		logLum += LumLog( g_TexSrc.Sample(g_SamplerSrc, tex[i].zw).rgb );
	}

	logLum /= 4.0;
	
	return float4(logLum, logLum, logLum, 1.0f);
}	
	
