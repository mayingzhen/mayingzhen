// transformations
//float4x4 mViewProj: VIEWPROJECTION;
float4x4 worldviewprojection: WORLDVIEWPROJECTION;

float4 posOffset : posScaleOffset = {1.0, 1.0, 0.0f, 0.0f};
float4 ambient_light = {1.0f, 1.0f, 1.0f, 1.0f};
float4 sun_vec: sunlight_vec = {0.0f, 0.0f, 0.0f, 1.0f};

float4 diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
float4 specular = {0.0f, 0.0f, 0.0f, 1.0f};

texture TerrainTex : TEXTURE; 
sampler LinearSamp0 = sampler_state 
{
    texture = <TerrainTex>;
    //AddressU  = wrap;        
    //AddressV  = wrap;
    //AddressW  = wrap;
    AddressU  = clamp;        
    AddressV  = clamp;
    AddressW  = clamp;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


texture g_TextureLightDiffuse;
sampler g_SamplerLightDiffuse = sampler_state
{
	Texture = <g_TextureLightDiffuse>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

texture g_TextureLightSpecular;
sampler g_SamplerLightSpecular = sampler_state
{
	Texture = <g_TextureLightSpecular>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};


struct VS_INPUT
{
	float4	Pos		: POSITION0;
	float2	UV1		: TEXCOORD0;
	float2	UV2		: TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 vTex0  : TEXCOORD0;
    float2 vTex1  : TEXCOORD1; 
    float2 oTc : TEXCOORD2;
 
};

VS_OUTPUT VS(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(v.Pos, worldviewprojection);
	Out.vTex0 = v.UV1;
	Out.vTex1 = v.UV2;
	
	Out.oTc = Out.Pos.xy / Out.Pos.w;
	Out.oTc.y *= -1;
	Out.oTc = Out.oTc * 0.5f + 0.5f;
	
    return Out;
}

//     float4 c0 = tex2D(layer0, uv0);
//     float4 c1 = tex2D(layer1, uv1);
//     float3 texturedColour = lerp(c0.rgb, c1.rgb, c1.a);
//     float4 lightmapColour = tex2D(lightmap, uvLightmap);
//     float4 baseColour = diffuse * lightmapColour;
//     float3 finalColour = baseColour.rgb * texturedColour + specular.rgb * (1-c0.a) * (1-c1.a) * lightmapColour.a;
//     //float3 resultColour = Fogging(finalColour);
//     oColour = float4(finalColour, baseColour.a);
float4 ThreeSurfaceBlend(VS_OUTPUT In) : COLOR
{	 
	float3 LightDiffuse = tex2D(g_SamplerLightDiffuse, In.oTc);
	float3 LightSpecular = tex2D(g_SamplerLightSpecular, In.oTc);

	In.vTex0 = saturate(In.vTex0);
	In.vTex1 = saturate(In.vTex1);
	float4 texColor0 = tex2Dgrad(LinearSamp0, In.vTex0, ddx(In.vTex0), ddy(In.vTex0) ); 
	float4 texColor1 = tex2Dgrad(LinearSamp0, In.vTex1, ddx(In.vTex1), ddy(In.vTex1) ); 
	
	float3 texColor = lerp(texColor0.rgb, texColor1.rgb, texColor1.a);
	float4 baseColor = diffuse;
	//float3 finalColor = baseColor.rgb * texColor + 
	//					specular.rgb * (1 - texColor0.a ) * (1 - texColor1.a );		
	///Terrrain specular 
	float3 finalColor = LightDiffuse * texColor ;//+ LightSpecular * (1 - texColor0.a ) * (1 - texColor1.a );	
															
	return float4(finalColor,texColor1.a);
}


technique SinglePassTerrain
{
    pass P0
    {
		//FILLMODE = WIREFRAME;
 		//CULLMODE = CW;
 		
        // shaders
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 ThreeSurfaceBlend();
    }  
}

