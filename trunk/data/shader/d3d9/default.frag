#ifdef DIFFUSE   
uniform sampler2D u_texture;
#endif

#ifdef DeferredLight   
uniform sampler2D u_textureLightDiffuse;
uniform sampler2D u_textureLightSpecular;
#endif

#ifdef DIFFUSECOLOR   
uniform float4 u_cDiffuseColor; 
#endif

// Varyings
struct PS_IN
{
#ifdef DIFFUSE
   float2   v_texCoord : TEXCOORD0;
#endif

#ifdef DeferredLight
   float2 v_defTc : TEXCOORD1;
#endif
   
#ifdef COLOR      
   float4   v_color : COLOR0;
#endif   
};


float4 main(PS_IN In) : COLOR0
{
#ifdef DIFFUSECOLOR   
    float4 flagColor = u_cDiffuseColor;
#else
	float4 flagColor = float4(1,1,1,1);
#endif   
   
#ifdef COLOR   
   flagColor = In.v_color;   
#endif


#ifdef DIFFUSE
   flagColor *= tex2D(u_texture, In.v_texCoord);
#endif 


#ifdef DeferredLight
   float3 LightDiffuse = tex2D(u_textureLightDiffuse, In.v_defTc);
   float3 LightSpecular = tex2D(u_textureLightSpecular, In.v_defTc);
   
   flagColor = float4( LightDiffuse.rgb * flagColor.rgb + LightSpecular.rgb, 1.0f );
#endif

   return flagColor; 
}
