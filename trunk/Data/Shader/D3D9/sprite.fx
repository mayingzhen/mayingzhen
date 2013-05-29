float4x4 u_projectionMatrix : projection;


texture u_texture;
sampler g_SamplerSrcDiffuse = sampler_state
{
	Texture = <u_texture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

void ShadingVS( float4 pos : POSITION,
				float2 texcoord : TEXCOORD0,
				out float4 oPos : POSITION,
				out float2 oTexCoord: TEXCOORD0)
{
	oPos = mul( pos, u_projectionMatrix );
	oTexCoord = texcoord;
}

float4 ShadingPS( float2 texcoord : TEXCOORD0) : COLOR0
{
	float4 srcDiffuse = tex2D(g_SamplerSrcDiffuse, texcoord);
	
	//return float4(1.0f,0.0f,0.0f,1.0f);
	return float4( srcDiffuse.rgb, 1.0f);
}




technique sprite
{
	pass P0
	{
		VertexShader = compile vs_2_0 ShadingVS();
		PixelShader = compile ps_2_0 ShadingPS();
	}
};

