float2 src_tex_size;

float color_weight[8];

sampler2D g_SamplerSrc;



float4 CalcBlur(float4 iTex0, float4 iTex1, float4 iTex2, float4 iTex3, float2 offset)
{
	float4 color = float4(0, 0, 0, 1);
	float4 tex[4] = { iTex0, iTex1, iTex2, iTex3 };

	for (int i = 0; i < 4; ++ i)
	{
		tex[i] += offset.xyxy;
		color.rgb += tex2D(g_SamplerSrc, tex[i].xy).rgb * color_weight[i * 2 + 0];
		color.rgb += tex2D(g_SamplerSrc, tex[i].zw).rgb * color_weight[i * 2 + 1];
	}

	return color;
}

float4 main(float4 iTex0 : TEXCOORD0,
				float4 iTex1 : TEXCOORD1,
				float4 iTex2 : TEXCOORD2,
				float4 iTex3 : TEXCOORD3,
				float2 iOriTex : TEXCOORD4) : COLOR
{

#if BLUR_X == 1
	float2 offset = float2((floor(iOriTex.x * src_tex_size.x) + 0.5f) * src_tex_size.y - iOriTex.x, 0);
#elif BLUR_Y == 1
	float2 offset = float2(0, (floor(iOriTex.y * src_tex_size.x) + 0.5f) * src_tex_size.y - iOriTex.y);
#endif
		
	return CalcBlur(iTex0, iTex1, iTex2, iTex3, offset);
}

