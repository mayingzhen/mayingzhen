// transformations
//float4x4 mViewProj: VIEWPROJECTION;
float4x4 mWorldViewProj: WORLDVIEWPROJECTION;


texture frameTex : TEXTURE; 
sampler LinearSamp0 = sampler_state 
{
    texture = <frameTex>;
    AddressU  = wrap;        
    AddressV  = wrap;
    AddressW  = wrap;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture depthTex : TEXTURE; 
sampler LinearSamp1 = sampler_state 
{
    texture = <depthTex>;
    AddressU  = wrap;        
    AddressV  = wrap;
    AddressW  = wrap;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
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
};

VS_OUTPUT VS(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(v.Pos, mWorldViewProj);
	Out.vTex0 = v.UV1;
	Out.vTex1 = v.UV2;
	
    return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{   
	float4 texColor0 = tex2D(LinearSamp0, In.vTex0 ); 
	float4 texColor1 = tex2D(LinearSamp1, In.vTex1 ); 
	
	float4 finalColor = texColor0 * texColor1;					
	return finalColor;
}    


technique TerrainLiquid
{
    pass P0
    {
		//FILLMODE = WIREFRAME;
 		ZENABLE = TRUE;
 		ZWRITEENABLE = false;
 		ZFUNC = LESSEQUAL; 
 			
 		AlphaBlendEnable = true;
 		SrcBlend = SrcAlpha;
 		DestBlend = InvSrcAlpha;
 		BlendOp = Add;
 		SrcBlendAlpha = One;
 		DestBlendAlpha = Zero;
 		BlendOpAlpha = Add;
 		
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
}
