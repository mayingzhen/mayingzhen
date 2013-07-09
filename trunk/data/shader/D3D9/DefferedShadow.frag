float4 depth_near_far_invfar;


float4x4 viwToLightProjArray[4];
float4 splitPos;


sampler2D SamplerShadowMap0;
sampler2D SamplerShadowMap1;
sampler2D SamplerShadowMap2;
sampler2D SamplerShadowMap3 = sampler_state

sampler g_SamplerSrcPos;


struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};


void GetPos(VS_OUT In,out float3 pos_es)
{
   float depth = tex2D(u_TextureSrcPos, In.oTc).x;
   depth *= depth_near_far_invfar.y;
   
   float3 view_dir = normalize(In.oViewDir);
   pos_es = view_dir * (depth / view_dir.z);   
}


void main( VS_OUT In,out float4 color : COLOR0 )
{
	float3 pos_es;
	GetPos(In,pos_es);
	
	float shadow = 1;
		
	if ( depth < splitPos.x )
	{
		float4 projCoord = mul( viwToLightProjArray[0] , float4( pos_es, 1 ) );
		
		shadow = tex2Dproj(SamplerShadowMap0, projCoord) ;	
	}
	else if ( depth < splitPos.y )
	{
		float4 projCoord = mul( viwToLightProjArray[1], float4( pos_es, 1 ) );
		
		shadow = tex2Dproj(SamplerShadowMap1, projCoord) ;	
	}
	else if ( depth < splitPos.z )
	{
		float4 projCoord = mul( viwToLightProjArray[2], float4( pos_es, 1 ) );
		
		shadow = tex2Dproj(SamplerShadowMap2, projCoord) ;	
	}
	else
	{
		float4 projCoord = mul( viwToLightProjArray[3], float4( pos_es, 1 ) );
		
		shadow = tex2Dproj(SamplerShadowMap3, projCoord) ;		
	}			
	
	color = shadow;
}






	


