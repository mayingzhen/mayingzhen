
Texture2D g_TexSrc;
SamplerState g_SamplerSrc;

float4 main(float2 tex : TEXCOORD0) : SV_TARGET
{
	float4 clr = g_TexSrc.Sample(g_SamplerSrc, tex);
	
	float lum = dot( float3(0.3333, 0.3333, 0.3333), clr.rgb );
	
	float gThreshold = 0.8f;
	
	float3 color = clr.rgb * (lum - gThreshold) / (1.0f - gThreshold);
	
	color = max(float3(0, 0, 0), color);
	
	return float4(color, 1);

}

