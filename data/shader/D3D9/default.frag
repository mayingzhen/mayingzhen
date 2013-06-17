#ifdef DIFFUSE   
uniform sampler2D u_texture;
#endif

// Varyings
struct PS_IN
{
#ifdef DIFFUSE
   float2   v_texCoord : TEXCOORD0;
#endif
   
#ifdef COLOR      
   float4   v_color : COLOR0;
#endif   
};


float4 main(PS_IN In) : COLOR0
{
   float4 flagColor = 1;
   
#ifdef COLOR   
   flagColor = In.v_color;   
#endif

#ifdef FONT
   flagColor.a *= tex2D(u_texture, In.v_texCoord).a;	
#else

#ifdef DIFFUSE
  flagColor *= tex2D(u_texture, In.v_texCoord);
#endif 

#endif
       
   return flagColor; 
}

