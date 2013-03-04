float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

float4 depth_near_far_invfar;
float viewMaxDepth[4];

float4x4 viwToLightProjArray[4];
float4x4 wordLightView[4];
float4 splitPos;


#define MAX_SHADOW_SAMPLES_NUM 16

float4 vInvShadowMapWH = float4(1.0f / 1024.0f, 1.0f / 1024.0f, 0, 0);	
float4 irreg_kernel_2d[MAX_SHADOW_SAMPLES_NUM / 2];
float4 g_vViewPos = float4(0,0,0,1);

//float4 fDepthTestBias;
//float4 fShadowFadingDist;

static const float2 KernelSize = float2(1.1f, 1.1f);
static const int nShadowSamplesNum = 8;

texture rotrandomTex;
sampler2D sRotSampler = sampler_state
{
	Texture = <rotrandomTex>; //<Textures/Defaults/rotrandom.dds>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE; 
	AddressU = Wrap;
	AddressV = Wrap;	
};


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


void DoubleSampleRotated(sampler2D depthMap, float4 p, float4 rotMatr, float4 kernel, out half2 result)
{
	// Rotate tap for this pixel location
	float4 rotatedOff;
	
	rotatedOff = rotMatr.xyzw * kernel.xxww + rotMatr.zwxy * kernel.yyzz;
	result.x = tex2Dproj( depthMap, p + (rotatedOff.xyzw * float4(1, 1, 0, 0 ) ) ).r;
	result.y = tex2Dproj( depthMap, p + (rotatedOff.zwxy * float4(1, 1, 0, 0 ) ) ).r;	
}

void irregular_filter(sampler2D depthMap, float4 shadowSapcePos, float2 randDirTc, 
             half2 kernelRadius,  out half shadowTest)	
{
	half kernelOffset = 0;
	
	half kernelSize = nShadowSamplesNum / 2;
	
	//half P_Z = p.z;
	float4 p0 = float4(shadowSapcePos.xyz, 1.0f);
	
	half2 rotScale = kernelRadius.y * 15.0f;
	
	shadowTest = 0;
	
	#define KERNEL_STEP_SIZE 2
	float2 rotSample = 2.0f * tex2D(sRotSampler, randDirTc.xy * rotScale.xy).xy - 1.0f;
	rotSample.xy = normalize(rotSample.xy);
	rotSample.xy *= (kernelRadius.xy * vInvShadowMapWH.xy);
	
	float4 rot = float4(rotSample.x, -rotSample.y, rotSample.y, rotSample.x);
	
	for (int i = kernelOffset; i < kernelSize; i += KERNEL_STEP_SIZE)
	{
		half4 sampleDepth;
		DoubleSampleRotated(depthMap, p0, rot, irreg_kernel_2d[i+0], sampleDepth.xy);
		DoubleSampleRotated(depthMap, p0, rot, irreg_kernel_2d[i+0], sampleDepth.zw);
		
		half4 InShadow = sampleDepth;
		half fInvSamplNum = (1.0 / nShadowSamplesNum );
		shadowTest += dot( InShadow, fInvSamplNum.xxxx );
	}
}


float2 GetNoisePos(float3x3 RotMatrix, float sceneDepth, float3 view_dir)
{
	float3 viewPosLS = mul( g_vViewPos.xyz, RotMatrix );
	float3 viewVectLS = mul( g_vViewPos.xyz + view_dir, RotMatrix ) -  viewPosLS.xyz;
	
	return viewPosLS.xy + viewVectLS.xy * sceneDepth; 	
}


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
		//float3x3 RotMatrix = (float3x3)viwToLightProjArray[0];
		//float2 noisePos = GetNoisePos(RotMatrix, depth, view_dir);
		//irregular_filter(SamplerShadowMap0, projCoord, noisePos, KernelSize, shadow);
	}
	else if ( depth < splitPos.y )
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[1] );
		
		shadow = tex2Dproj(SamplerShadowMap1, projCoord) ;	
		//float3x3 RotMatrix = (float3x3)viwToLightProjArray[1];
		//float2 noisePos = GetNoisePos(RotMatrix, depth, view_dir);
		//irregular_filter(SamplerShadowMap1, projCoord, noisePos, KernelSize, shadow);
	}
	else if ( depth < splitPos.z )
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[2] );
		
		shadow = tex2Dproj(SamplerShadowMap2, projCoord) ;	
		//float3x3 RotMatrix = (float3x3)viwToLightProjArray[2];
		//float2 noisePos = GetNoisePos(RotMatrix, depth, view_dir);
		//irregular_filter(SamplerShadowMap2, projCoord, noisePos, KernelSize, shadow);
	}
	else
	{
		float4 projCoord = mul( float4( pos_es, 1 ), viwToLightProjArray[3] );
		
		shadow = tex2Dproj(SamplerShadowMap3, projCoord) ;	
		//float3x3 RotMatrix = (float3x3)viwToLightProjArray[3];
		//float2 noisePos = GetNoisePos(RotMatrix, depth, view_dir);
		//irregular_filter(SamplerShadowMap3, projCoord, noisePos, KernelSize, shadow);		
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





	


