float4x4 viewprojection : viewprojection;

static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

int CurNumBones = 2;

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



void SkinPos( float4 pos ,
			    float4 BlendWeights , 
				float4 BlendIndices,
				out float3 wPos)
{
	wPos = 0;
	float LastWeight = 0.0f;
	// cast the vectors to arrays for use in the for loop below
	int4 Indices = D3DCOLORtoUBYTE4(BlendIndices);
    int   IndexArray[4]   = (int[4])Indices; 
    float WeightsArray[4] = (float[4])BlendWeights;
    for (int iBone = 0; iBone < CurNumBones-1; iBone++)
    {
        LastWeight = LastWeight + WeightsArray[iBone];   
        wPos += mul(pos, mWorldMatrixArray[IndexArray[iBone]]) * WeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 
    // Now that we have the calculated weight, add in the final influence
    wPos += (mul(pos, mWorldMatrixArray[IndexArray[CurNumBones-1]]) * LastWeight);
}

void SkinShadingVS( float4 pos : POSITION,
					float4 BlendWeights :BLENDWEIGHT, 
					float4 BlendIndices :BLENDINDICES,
					float2 texcoord : TEXCOORD0,
					out float4 oPos : POSITION,
					out float2 oTexCoord: TEXCOORD0,
					out float2 oTc : TEXCOORD1)
{
	float3 wPos = 0;

  	SkinPos(pos,BlendWeights,BlendIndices,wPos);
	
	oPos = mul( float4(wPos.xyz, 1.0f), viewprojection );
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


technique SkinShading
{
	pass P0
	{
		VertexShader = compile vs_3_0 SkinShadingVS();
		PixelShader = compile ps_3_0 ShadingPS();
		
		//cu
	}
};