Texture2D tSrcColor;
SamplerState sSrcColor;

float4 tex_coord_offset[2];

float4 main(float2 itex : TEXCOORD0) : SV_TARGET
{
	float4 tex[2];
	
	tex[0] = itex.xyxy + tex_coord_offset[0];
	tex[1] = itex.xyxy + tex_coord_offset[1];

	float s = 0;
	for (int i = 0; i < 2; ++ i)
	{
		s += tSrcColor.Sample(sSrcColor, tex[i].xy).r;
		s += tSrcColor.Sample(sSrcColor, tex[i].zw).r;
	}

	s /= 4.0;
	
	return float4(s, s, s, 1.0f);
}
	
