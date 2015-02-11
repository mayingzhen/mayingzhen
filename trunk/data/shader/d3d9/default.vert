// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include "common.h"

#ifdef SKIN
#include "skin.h"
#endif 

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

// Attributes
struct VS_IN
{
   float3 a_position : POSITION;

#ifdef SKIN   
   float4 a_blendWeights :BLENDWEIGHT;     
   int4   a_blendIndices :BLENDINDICES;
#endif   
 
#ifdef DIFFUSE   
   float2 a_texCoord0 : TEXCOORD0;
#endif

	float3 a_normal	 : NORMAL;

#ifdef COLOR   
   float4 a_color0 : COLOR0;
#endif

};


// Varyings
struct VS_OUT
{
   float4 v_position : POSITION;

#ifdef DIFFUSE    
   float2 v_texCoord : TEXCOORD0;
#endif  

#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD1;	
#endif   
   
#ifdef COLOR      
   float4 v_color : COLOR0;
#endif

};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
	float3 iPos  = In.a_position;
	float3 iNormal = In.a_normal; 

#ifdef SKIN
	SkinPosNormal(In.a_position,In.a_normal,In.a_blendIndices,In.a_blendWeights,iPos,iNormal);
#endif
   
    Out.v_position = mul(float4(iPos,1.0),g_matWorldViewProj); 
    
#ifdef DIFFUSE      
    Out.v_texCoord = In.a_texCoord0;
#endif
   
#ifdef COLOR    
    Out.v_color = In.a_color0;
#endif

#ifdef DEFERREDSHADING 
	GBufferVSOut(iNormal,Out.v_position.w,Out.v_normalDepth);
#endif  

    return Out;
}


