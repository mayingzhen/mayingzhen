
#include "common.h"


struct ObjectPS
{
    float4 u_cDiffuseColor;
};

/*
struct Sampler
{
    texture2d<float> u_texture [[texture(0)]];
    sampler s_texture [[sampler(0)];
};
*/

// Varyings
struct VS_OUT
{
    float4 v_position [[position]];
    
    float2   v_texCoord ;

	float4 WorldPos ;
	float4 worldNormal;

#ifdef DEFERREDSHADING 
    float3 viewNormal ;
#else  

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float2 oRandDirTC ;
	float4 oShadowPos ;
#endif
#endif 
   
};


fragment float4 ps_main(VS_OUT In [[stage_in]]
                , constant ObjectPS &constants[[buffer(0)]]
                 , texture2d<float> u_texture [[texture(0)]]
                 , sampler s_texture [[sampler(0)]]
) 
{
    float4 gl_FragColor;
    
#ifdef DIFFUSECOLOR
    gl_FragColor = constants.u_cDiffuseColor;
#else
    gl_FragColor = float4(1.0,1.0,1.0,1.0);
#endif
    
#ifdef COLOR
    gl_FragColor = v_color;
#endif
    
    gl_FragColor *=  u_texture.sample(s_texture, In.v_texCoord);
    
    return gl_FragColor;
}
