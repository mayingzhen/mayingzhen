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

texture g_TextureLum;
sampler g_SamplerLum = sampler_state
{
	Texture = <g_TextureLum>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture g_TextureBloom;
sampler g_SamplerBloom = sampler_state
{
	Texture = <g_TextureBloom>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1;
	tex += 0.5;
	return tex;
}

void PostProcessVS(float4 pos : POSITION,
					out float2 oTex : TEXCOORD0,
					out float4 oPos : POSITION)
{
	oTex = TexCoordFromPos(pos);
	oPos = pos;
}


float EyeAdaption(float lum)
{
	return lerp(0.2f, lum, 0.5f);
}

void ToneMappingVS(float4 pos : POSITION,
					out float3 oTex : TEXCOORD0,
					out float4 oPos : POSITION)
{
	oPos = pos;

	oTex.xy = TexCoordFromPos(pos);
	float lum = tex2D(g_SamplerLum, float2(0.5f, 0.5f) ).r;
	oTex.z = max(0.001f, lum);
}


float3 F(float3 x)
{
	//const float A = 0.22f;
	//const float B = 0.30f;
	//const float C = 0.10f;
	//const float D = 0.20f;
	//const float E = 0.01f;
	//const float F = 0.30f;
	
	const float A = 0.15f;
	const float B = 0.50f;
	const float C = 0.10f;
	const float D = 0.20f;
	const float E = 0.02f;
	const float F = 0.30f;

	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}


float3 ToneMapping(float3 color, float3 blur, float adapted_lum)
{
	const float3 RGB_TO_LUM = float3(0.2126f, 0.7152f, 0.0722f);
	const float3 BLUE_SHIFT = float3(0.4f, 0.4f, 0.7f);

	color += blur * 0.25f;
	
	float lum = dot(color, RGB_TO_LUM);
	
	// martin's modified blue shift
	color = lerp(lum * BLUE_SHIFT, color, saturate(16.0f * lum));

	float adapted_lum_dest = 3 / (max(0.1f, 1 + 10 * EyeAdaption(adapted_lum)));

	// Filmic Tonemapping from Unchart 2
	const float White = 11.2f;
	return F(1.6f * adapted_lum_dest * color) / F(White);
}


/*
// Exponentional mappping 
half4 ExpMapping( in vert2frag IN, in half4 cScene, in half4 cBloom, in half fAdaptedLum, in half fVignetting )
{
	half fLum = GetLuminance( cScene.rgb );
	fAdaptedLum = EyeAdaption( fAdaptedLum );

	// Exposure
	// Crysis 1 defaults: HDRParams1.y = 3, HDRParams1.z = 10

#if !%_RT_SAMPLE4
  half fAdaptedLumDest = HDRParams1.y / (1e-6 + 1.0h + HDRParams1.z * fAdaptedLum);
#else
  // pre-computing on cpu instead
	half fAdaptedLumDest = HDRParams1.y;
#endif

  cScene = ApplyBlueShift( IN, cScene, fLum );

#if USE_LUM_CONTRAST
	// Adjustable contrast in low luminance areas
	//cScene.xyz = lerp( pow( cScene.xyz, HDRParams2.z ), cScene.xyz, saturate( fLum * HDRParams2.w ) );

  // a bit more optimal version
  half fLumLerp = saturate( fLum );
  cScene.xyz= pow(cScene.xyz,HDRParams2.z + fLumLerp* ( 1 - HDRParams2.z) );
#endif

	// Tone mapping
	cScene.xyz = 1 - exp2( -fVignetting * (fAdaptedLumDest * cScene.xyz +cBloom  ));

	return cScene;
}
*/



/*
float3 ToneMapping(float3 color, float3 blur, float adapted_lum)
{
	const float3 RGB_TO_LUM = float3(0.27, 0.67, 0.06);
	const float3 BLUE_SHIFT = float3(0.8f, 0.8f, 1.4f);

	color += blur * 0.25f;
	
	float lum = dot(color, RGB_TO_LUM);
	
	// martin's modified blue shift
	color = lerp(lum * BLUE_SHIFT, color, saturate(5.0f * lum));

	// tone mapping
	float adapted_lum_dest = 3 / (max(0.1f, 1 + 10 * EyeAdaption(adapted_lum)));

	//color = 1 - exp(-adapted_lum_dest * pow(color, 1.15f)); // better contrast but slower
	color = 1 - exp(-adapted_lum_dest * color); 

	return color;
}
*/


float4 ToneMappingPS(float2 iTex : TEXCOORD0) : COLOR
{	
	float3 color = tex2D(g_SamplerSrc, iTex.xy).rgb;
	float3 blur = tex2D(g_SamplerBloom, iTex.xy).rgb;
	float lum = max(0.001f, tex2D(g_SamplerLum, float2(0.5f, 0.5f) ).r );	
	
	//return float4( ToneMapping(color, blur, lum), 1 );
	return float4(color,1);
	
	//return( F(color) / F(1.2f), 1 );
}

technique ToneMaping
{
	pass P0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader = compile ps_3_0 ToneMappingPS();
		
		ZEnable = false;
		zWriteEnable = true;
		
		CullMode = cw;
	}
}


