
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

//static const float  g_fMiddleGray = 0.18f;
//static const float  BRIGHT_PASS_THRESHOLD  = 5.0f;  // Threshold for BrightPass filter
//static const float  BRIGHT_PASS_OFFSET     = 10.0f; // Offset for BrightPass filter

static const float  g_fMiddleGray = 1.72f;
static const float  BRIGHT_PASS_THRESHOLD  = 5.0f;  // Threshold for BrightPass filter
static const float  BRIGHT_PASS_OFFSET     = 10.0f; // Offset for BrightPass filter

float4 BrightPassDownsamplePS(float2 tex : TEXCOORD0) : COLOR
{
	//float4 clr = tex2D(g_SamplerSrc, tex);
	//return clr * (clr / 3);

	float4 vSample = tex2D(g_SamplerSrc, tex);
	float fAdaptedLum =  tex2D(g_SamplerLum, float2(0.5f, 0.5f) ).r; 
	
	// Determine what the pixel's value will be after tone-mapping occurs
	vSample.rgb *= g_fMiddleGray/(fAdaptedLum + 0.001f);
	
	// Subtract out dark pixels
	vSample.rgb -= BRIGHT_PASS_THRESHOLD;
	
	// Clamp to 0
	vSample = max(vSample, 0.0f);
	
	// Map the resulting value into the 0 to 1 range. Higher values for
	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
	// objects.
	vSample.rgb /= (BRIGHT_PASS_OFFSET+vSample);
    
	return vSample;	
	
}

technique BrightPassDownsample
{
	pass P0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader = compile ps_3_0 BrightPassDownsamplePS();
		
		CullMode = cw;
	}
}