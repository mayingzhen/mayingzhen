
sampler2D TerrainTex : TEXTURE; 

#ifdef DeferredLight   
uniform sampler2D u_textureLightDiffuse;
uniform sampler2D u_textureLightSpecular;
#endif

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 vTex0  : TEXCOORD0;
    float2 vTex1  : TEXCOORD1; 
    
#ifdef DeferredLight
   float2 v_defTc : TEXCOORD2;
#endif
 
};


float4 main(VS_OUTPUT In) : COLOR
{	 
	In.vTex0 = saturate(In.vTex0);
	In.vTex1 = saturate(In.vTex1);
	float4 texColor0 = tex2Dgrad(TerrainTex, In.vTex0, ddx(In.vTex0), ddy(In.vTex0) ); 
	float4 texColor1 = tex2Dgrad(TerrainTex, In.vTex1, ddx(In.vTex1), ddy(In.vTex1) ); 
	
	float3 texColor = lerp(texColor0.rgb, texColor1.rgb, texColor1.a);														
	//return float4(texColor,texColor1.a);
	
#ifdef DeferredLight
   float3 LightDiffuse = tex2D(u_textureLightDiffuse, In.v_defTc);
   float3 LightSpecular = tex2D(u_textureLightSpecular, In.v_defTc);
   
   texColor = LightDiffuse.rgb * texColor.rgb /*+ LightSpecular.rgb*/;
#endif
	
	return float4(texColor,texColor1.a);		
}


