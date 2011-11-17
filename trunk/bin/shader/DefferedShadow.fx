float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

float4 depth_near_far_invfar;
float viewMaxDepth[4];

float4x4 viwToLightProjArray[4];
float4x4 wordLightView[4];
float4 splitPos;

texture TexShadowMap0;
sampler2D SamplerShadowMap0 = sampler_state
{
	Texture = <TexShadowMap0>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture TexShadowMap1;
sampler2D SamplerShadowMap1 = sampler_state
{
	Texture = <TexShadowMap1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture TexShadowMap2;
sampler2D SamplerShadowMap2 = sampler_state
{
	Texture = <TexShadowMap2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture TexShadowMap3;
sampler2D SamplerShadowMap3 = sampler_state
{
	Texture = <TexShadowMap3>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};


texture g_TextureSrcPos;
sampler g_SamplerSrcPos = sampler_state
{
	Texture = <g_TextureSrcPos>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};

/*
texture Jitter;
sampler JitterSampler = sampler_state
{
    Texture = <Jitter>;
    MinFilter = Point;  
    MagFilter = Point;
    MipFilter = None;
    AddressU  = Wrap;
    AddressV  = Wrap;
};
*/

void DefferedShadowVS( float4 pos : POSITION,
					   out float4 oPos : POSITION,
					   out float2 oTc : TEXCOORD0,
					   out float3 oViewDir : TEXCOORD1)
{
	oPos = mul( float4(pos.xyz ,1), worldviewprojection );
	
	oViewDir = mul( float4(pos.xyz ,1), worldview ).xyz;
	
	oTc = oPos.xy / oPos.w * 0.5f;
	oTc.y *= -1;
	oTc += 0.5f;
}


void DefferedShadowPS( float2 Tc : TEXCOORD0,
					   float3 ViewDir : TEXCOORD1,
					   out float4 color : COLOR0 )
{
	half depth = tex2D(g_SamplerSrcPos, Tc).x;
	depth *= depth_near_far_invfar.y;
	
	float3 view_dir = normalize(ViewDir);
	float3 pos_es = view_dir * (depth / view_dir.z); 
	
	float shadow = 1;
		
	if ( depth < splitPos.x )
	{
	
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[0] );
		shadow = tex2Dproj(SamplerShadowMap0, projCoord) ;	
	}
	else if ( depth < splitPos.y )
	{
		
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[1] );
		shadow = tex2Dproj(SamplerShadowMap1, projCoord) ;	
	}
	else if ( depth < splitPos.z )
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[2] );
		shadow = tex2Dproj(SamplerShadowMap2, projCoord) ;	
	}
	else
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[3] );
		shadow = tex2Dproj(SamplerShadowMap3, projCoord) ;		
	}			
	
	color = shadow;
}


/* for pix debug shader
static const float fShadowBias = 0.001f;
void DefferedShadowPS( float2 Tc : TEXCOORD0,
					   float3 ViewDir : TEXCOORD1,
					   out float4 color : COLOR0 )
{	
	half depth = tex2D(g_SamplerSrcPos, Tc).x;
	depth *= depth_near_far_invfar.y;
	
	float3 view_dir = normalize(ViewDir);
	float3 pos_es = view_dir * ( depth / view_dir.z ); 
	
	float shadow = 1;
	
	if ( depth < splitPos.x )
	{
	
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[0] );
		float4 ligtViewPos = mul( float4( pos_es, 1 ), wordLightView[0] );
		
		float shadowDepth = tex2D(SamplerShadowMap0, projCoord.xyz / projCoord.w) + fShadowBias;
		shadowDepth *= depth_near_far_invfar.w;
		shadow = ligtViewPos.z < shadowDepth;
	}
	else if ( depth < splitPos.y )
	{
		
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[1] );
		float4 ligtViewPos = mul( float4( pos_es, 1 ), wordLightView[1] );
		float shadowDepth = tex2D(SamplerShadowMap1, projCoord.xyz / projCoord.w ) + fShadowBias;
		shadowDepth *= depth_near_far_invfar.w;
		shadow = ligtViewPos.z < shadowDepth;
	}
	else if ( depth < splitPos.z )
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[2] );
		float4 ligtViewPos = mul( float4( pos_es, 1 ), wordLightView[2] );
		float shadowDepth = tex2D(SamplerShadowMap2, projCoord.xyz / projCoord.w ) + fShadowBias;
		shadowDepth *= depth_near_far_invfar.w;
		shadow = ligtViewPos.z < shadowDepth;	
	}
	else
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[3] );
		float4 ligtViewPos = mul( float4( pos_es, 1 ), wordLightView[3] );
		float shadowDepth = tex2D(SamplerShadowMap3, projCoord.xyz / projCoord.w ) + fShadowBias;
		shadowDepth *= depth_near_far_invfar.w;
		shadow = ligtViewPos.z < shadowDepth;	
	}		
	
	color = shadow;
}
*/

technique DefferedShadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 DefferedShadowVS();
		PixelShader = compile ps_3_0  DefferedShadowPS();
		
		CullMode = cw;
	}		
}