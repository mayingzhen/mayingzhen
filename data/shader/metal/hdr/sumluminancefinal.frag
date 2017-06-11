float4 tex_coord_offset[2];

sampler2D g_SamplerSrc;


float4 main(float2 itex : TEXCOORD0) : COLOR
{
	float4 tex[2];
	
	tex[0] = itex.xyxy + tex_coord_offset[0];
	tex[1] = itex.xyxy + tex_coord_offset[1];

	float s = 0;
	for (int i = 0; i < 2; ++ i)
	{
		s += tex2D(g_SamplerSrc, tex[i].xy).r;
		s += tex2D(g_SamplerSrc, tex[i].zw).r;
	}

	s /= 4;
	
	s = exp(s);
   
    return float4(s, s, s, 1.0f);
}	

	
