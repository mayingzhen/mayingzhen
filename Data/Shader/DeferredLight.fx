float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

float4 light_pos_es;
float4 light_dir_es;
float4 light_color ;//= (0.2f,0.2f,0.2f,0.2f);
float  lightRadius;
float  g_OneOverSqrLightRadius;

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


float GetAttenuation(half3 lightVec)
{
  	//return saturate(1 -  dot(lightVec, lightVec) * g_OneOverSqrLightRadius );
  	//return saturate( 1 - lightVec.x * lightVec.x *  g_OneOverSqrLightRadius );
  	return saturate( 1 -  lightVec.x /  lightRadius );
}
		
struct VS_OUT
{
	float4 pos : POSITION;
	float2 oTc : TEXCOORD0;
	float3 oViewDir : TEXCOORD1;
};

struct PS_OUT
{
 	half4 Diffuse  : COLOR0;
	half4 Specular  : COLOR1;
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

void DeferredLightPS(VS_OUT In, out PS_OUT pOut)
{
	pOut = (PS_OUT)0;
	
	half depth = tex2D(g_SamplerSrcPos, In.oTc).x;
	depth *= depth_near_far_invfar.y;
	
	float3 view_dir = normalize(In.oViewDir);
	float3 pos_es = view_dir * (depth / view_dir.z);	
	
	half4 SrcNormal = tex2D( g_SamplerSrcNormal, In.oTc);
	half3 vNormal = SrcNormal.xyz * 2 - 1;
	half  shiness = SrcNormal.w  * 255.0f;

	half shadow = tex2D(g_SamplerShadow, In.oTc).r;
	shadow = 1.0f;
	
	//half3 vlightVec = light_pos_es.xyz - pos_es.xyz;
	half3 vlightVec = light_dir_es;
	
	half3 vLight  = normalize(vlightVec);	
	half3 vView   = normalize(-pos_es.xyz);
	half3 halfDir = normalize(vView + vLight);
	
	half3 light = lit( dot( vLight, vNormal ), dot( halfDir, vNormal ), shiness );	
	
	//vlightVec.x = distance( light_pos_es, pos_es );
	//half fAttenuation = GetAttenuation(vlightVec);
	half fAttenuation = 1.0f; // Directon light 
	
	half3 cDiffUse = light.y * shadow * light_color * fAttenuation;
	half3 cSpecular = light.z * shadow * light_color * fAttenuation;
		
	pOut.Diffuse.xyz = cDiffUse;
	pOut.Specular.xyz = cSpecular;	
}


void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
	pOut = (PS_OUT)0;
	
	half3 cDiffuse = 0.2f;
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
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		
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


technique DiffuseLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 DeferredLightVS() ;
		PixelShader = compile ps_3_0 DeferredLightPS() ;
		
		//StencilEnable = false;
		//ZFunc = GreaterEqual;
		//CullMode = CCW;
		CullMode = CW;

		ZEnable = false;
		ZWriteEnable = false;
		
		//AlphaBlendEnable = true;
		//SrcBlend = One;
		//DestBlend = One;
	}
}

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
