float4 tex_coord_offset[2];

sampler2D g_SamplerSrc;


float4 main(float2 itex : TEXCOORD0) : COLOR
{
	const float3 RGB_TO_LUM = float3(0.299f, 0.587f, 0.114f);
	
	float4 tex[2] ;
	
	tex[0] = itex.xyxy + tex_coord_offset[0];
	tex[1] = itex.xyxy + tex_coord_offset[1];

	float logLum = 0;
	for (int i = 0; i < 2; ++ i)
	{
		logLum += log( dot( tex2D(g_SamplerSrc, tex[i].xy).rgb, RGB_TO_LUM) + 0.001f);
		logLum += log( dot( tex2D(g_SamplerSrc, tex[i].zw).rgb, RGB_TO_LUM) + 0.001f);
	}

	logLum /= 4;
	
	return float4(logLum, logLum, logLum, 1.0f);
}	
	
