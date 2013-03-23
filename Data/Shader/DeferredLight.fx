float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

float4 light_pos_es;
float4 light_dir_es;
float4 light_color ;//= (0.2f,0.2f,0.2f,0.2f);

float4 depth_near_far_invfar;

// Gbuffer
texture g_TextureSrcPos;
texture g_TextureSrcNormal;

// shadow
texture g_TextureShadow;

sampler g_SamplerSrcNormal = sampler_state
{
	Texture = <g_TextureSrcNormal>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler g_SamplerSrcPos = sampler_state
{
	Texture = <g_TextureSrcPos>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


sampler g_SamplerShadow = sampler_state
{
	Texture = <g_TextureShadow>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


		
struct VS_OUT
{
	float4 pos : POSITION;
	float2 oTc : TEXCOORD0;
	float3 oViewDir : TEXCOORD1;
};

struct PS_OUT
{
 	float4 Diffuse  : COLOR0;
	float4 Specular  : COLOR1;
};


void DeferredLightVS( float4 pos : POSITION,
					  out VS_OUT vOut)
{
	vOut.pos = mul( float4(pos.xyz ,1), worldviewprojection );
	
	vOut.oViewDir = mul( float4(pos.xyz ,1), worldview ).xyz;
	
	vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
	vOut.oTc.y *= -1;
	vOut.oTc += 0.5f;
}

void GetPosNormalShiness(VS_OUT In,out float3 pos_es,out float3 normal,out float shiness)
{
	float depth = tex2D(g_SamplerSrcPos, In.oTc).x;
	depth *= depth_near_far_invfar.y;
	
	float3 view_dir = normalize(In.oViewDir);
	pos_es = view_dir * (depth / view_dir.z);	
	
	float4 SrcNormal = tex2D( g_SamplerSrcNormal, In.oTc);
	normal = SrcNormal.xyz * 2 - 1;
	shiness = SrcNormal.w  * 255.0f;
}

void GetDiffuseSpecular(float3 lightVec, float3 pos_es, float3 normal,float shiness,out PS_OUT pOut)
{
	pOut = (PS_OUT)0;

	float3 vLight  = normalize(lightVec);	
	float3 vView   = normalize(-pos_es.xyz);
	float3 halfDir = normalize(vView + vLight);
	
	float3 light = lit( dot( vLight, normal ), dot( halfDir, normal ), shiness );	
	
	float3 cDiffUse = light.y * light_color;
	float3 cSpecular = light.z * light_color;
		
	pOut.Diffuse.xyz = cDiffUse;
	pOut.Specular.xyz = cSpecular;	
}

void DeferredDirectLightPS(VS_OUT In, out PS_OUT pOut)
{
	pOut = (PS_OUT)0;

	float3 pos_es;
	float3 normal;
	float shiness;
	GetPosNormalShiness(In,pos_es,normal,shiness);

	float3 vlightVec = light_dir_es;
	
	GetDiffuseSpecular(vlightVec,pos_es,normal,shiness,pOut);
}


void DeferredPointLightPS(VS_OUT In, out PS_OUT pOut)
{
	pOut = (PS_OUT)0;
	
	float3 pos_es;
	float3 normal;
	float shiness;
	GetPosNormalShiness(In,pos_es,normal,shiness);
	
	float3 vlightVec = light_pos_es.xyz - pos_es.xyz;
	
	GetDiffuseSpecular(vlightVec,pos_es,normal,shiness,pOut);
}



void DeferredShadowDirectLightPS(VS_OUT In, out PS_OUT pOut)
{
	DeferredDirectLightPS(In,pOut);	
	
	half shadow = tex2D(g_SamplerShadow, In.oTc).r;
	
	pOut.Diffuse *= shadow;
	pOut.Specular *= shadow;	
}

void DeferredShadowPointLightPS(VS_OUT In, out PS_OUT pOut)
{
	DeferredPointLightPS(In,pOut);	
	
	half shadow = tex2D(g_SamplerShadow, In.oTc).r;
	
	pOut.Diffuse *= shadow;
	pOut.Specular *= shadow;	
}




void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
	pOut = (PS_OUT)0;
	
	half3 cDiffuse = 0.3f;
	half3 cSpecular = 0;
		
	pOut.Diffuse.xyz = cDiffuse;
	pOut.Specular.xyz = cSpecular;	
}

void GemetryVS(float4 Pos : POSITION,out float4 oPos : POSITION)
{
	oPos = mul(Pos, worldviewprojection);
}


technique AmbientLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS();
    PixelShader = compile ps_3_0 AmbientLightPS();
        
		ZEnable = false;
		ZWriteEnable = false;
		
		//AlphaBlendEnable = true;
		//SrcBlend = One;
		//DestBlend = One;
		
		CullMode = CW;
	}
}


technique StencilVolumeMask
{
	pass p0
	{
		VertexShader = compile vs_1_1 GemetryVS();
		PixelShader = null;
		
        CullMode = None;

        ZEnable = true;
        ZWriteEnable = false;
        ZFunc = Less;
        
        // stencil states
        StencilEnable = true;
        StencilFail = keep;
        StencilZFail = Decr;
        StencilPass = Keep;
        StencilFunc = Always;    
        TwoSidedStencilMode = true;
        Ccw_StencilFail = keep;
        Ccw_StencilZFail = Incr;
        Ccw_StencilPass = Keep;
        Ccw_StencilFunc = Always;
        StencilRef = 1;
        StencilMask = 0xFFFFFFFF;
        StencilWriteMask = 0xFFFFFFFF;
	}
}


technique DiffuseDirectLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredDirectLightPS() ;
		
		//StencilEnable = false;
		//ZFunc = GreaterEqual;
		//CullMode = CCW;
		CullMode = CW;

		ZEnable = false;
		ZWriteEnable = false;
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
	}
}

technique DiffusePointLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredPointLightPS() ;
		
		ZEnable = false;
		ZWriteEnable = false;
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
	}
}

technique DiffuseShadowDirectLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredShadowDirectLightPS() ;
		
		//StencilEnable = false;
		//ZFunc = GreaterEqual;
		//CullMode = CCW;
		CullMode = CW;

		ZEnable = false;
		ZWriteEnable = false;
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
	}
}

technique DiffuseShadowPointLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredShadowPointLightPS() ;
		
		//CullMode = CW;

		ZEnable = false;
		ZWriteEnable = false;
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
	}
}


/*
technique DiffuseLightStencil
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredLightPS() ;
		
		ZEnable = true;
        ZFunc = LessEqual;
        
        StencilEnable = true;
        StencilFail = Keep;
        StencilZFail = keep;
        StencilPass = keep;
        StencilFunc = Greater;
        TwoSidedStencilMode = false;
        StencilRef = 1;
        StencilMask = 0xFFFFFFFF;
        StencilWriteMask = 0xFFFFFFFF;
        
        CullMode = CCW;
	}
}
*/
