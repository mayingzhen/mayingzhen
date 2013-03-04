// transformations
float4x4 mWorldViewProj: WORLDVIEWPROJECTION;
float4x4 world:world;

// the cube map
texture skyCubeMapTex : TEXTURE;

// cube map coordinates should not wrap
sampler LinearSamp0 = sampler_state 
{
    texture = <skyCubeMapTex>;
    //AddressU  = clamp;        
    //AddressV  = clamp;
    //AddressW  = clamp;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


void SkyVS( float4 Pos : POSITION,
		 	out float4 oPos : POSITION,
		 	out float3 oTc : TEXCOORD0 )
{
	oPos = mul(Pos, mWorldViewProj);
	oTc = normalize( mul(Pos, world) );
}


void SkyPs( float3 Tc:TEXCOORD0, out float4 color:COLOR0) 
{   
	color =  texCUBE(LinearSamp0, Tc); 
}  

technique BasicCubeMap
{
    pass P0
    {
    	 VertexShader = compile vs_2_0 SkyVS();
     	 PixelShader  = compile ps_2_0 SkyPs();
     	 
     	 //FillMode = WIREFRAME; 
     	 
     	 ZEnable = true;
         ZWriteEnable = true;
         AlphaBlendEnable = FALSE;
         //CullMode = ccw;
         AlphaTestEnable = FALSE;
    }  
}

