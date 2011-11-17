float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

texture g_TextureLightDiffuse;
texture g_TextureLightSpecular;

texture g_TextureSrcDiffuse;
texture g_TextureSrcSpecular;

sampler g_SamplerLightDiffuse = sampler_state
{
	Texture = <g_TextureLightDiffuse>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler g_SamplerLightSpecular = sampler_state
{
	Texture = <g_TextureLightSpecular>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler g_SamplerSrcDiffuse = sampler_state
{
	Texture = <g_TextureSrcDiffuse>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler g_SamplerSrcSpecular = sampler_state
{
	Texture = <g_TextureSrcSpecular>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};



void ShadingVS( float4 pos : POSITION,
				float2 texcoord : TEXCOORD0,
				out float4 oPos : POSITION,
				out float2 oTexCoord: TEXCOORD0,
				out float2 oTc : TEXCOORD1)
{
	oPos = mul( pos, worldviewprojection );
	oTexCoord = texcoord;
	
	oTc = oPos.xy / oPos.w;
	oTc.y *= -1;
	oTc = oTc * 0.5f + 0.5f;
}

float4 ShadingPS( float2 texcoord : TEXCOORD0,
				  float2 tc : TEXCOORD1) : COLOR0
{
	float4 srcDiffuse = tex2D(g_SamplerSrcDiffuse, texcoord);
	float3 srcSpecular = tex2D(g_SamplerSrcDiffuse, texcoord);
	
	float3 LightDiffuse = tex2D(g_SamplerLightDiffuse, tc);
	float3 LightSpecular = tex2D(g_SamplerLightSpecular, tc);
	
	//return float4(srcDiffuse.rgb, 1.0f);
	//return float4( max( (LightDiffuse * srcDiffuse + specular_normalize_factor(shininess, 0.04f) * lighting.a * specular), 0), 1);
	
	return float4( LightDiffuse.rgb * srcDiffuse.rgb + LightSpecular * srcSpecular, 1.0f );
}


technique Shading
{
	pass P0
	{
		VertexShader = compile vs_3_0 ShadingVS();
		PixelShader = compile ps_3_0 ShadingPS();
	}
};

