// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include "common.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

#ifdef DIFFUSE   
uniform sampler2D u_texture;
#endif


#ifdef DIFFUSECOLOR   
uniform float4 u_cDiffuseColor; 
#endif

float shininess = 16.0f;

// Varyings
struct PS_IN
{
#ifdef DIFFUSE
   float2   v_texCoord : TEXCOORD0;
#endif

#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD1;	
#endif
   
#ifdef COLOR      
   float4   v_color : COLOR0;
#endif   
};

float4 GetDiffuse(PS_IN In)
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

	return flagColor;
}

#ifndef DEFERREDSHADING 
float4 ForwardShading(float4 cDiffuse,PS_IN In)
{
	float4 flagColor = cDiffuse;

#if USING_SHADOW != 0	
	flagColor *= DoShadowMapping(In.ShadowPos,In.RandDirTC,In.WorldPos.w);
#endif
	
	return flagColor;
}
#endif

void main(PS_IN In,
#ifdef DEFERREDSHADING 
out PS_OUT pout
#else
out float4 outColor : COLOR0 
#endif
) 
{
	float4 cDiffuse = GetDiffuse(In); 

#ifdef DEFERREDSHADING 
	pout = GbufferPSout(cDiffuse,shininess,In.v_normalDepth);
#else
	outColor = ForwardShading(cDiffuse,In);  
#endif	
}
