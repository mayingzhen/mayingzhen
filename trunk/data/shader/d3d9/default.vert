#include "common.h"
#include "skin.h"


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

#ifdef DeferredLight
   float2 v_defTc : TEXCOORD1;
#endif

   
#ifdef COLOR      
   float4 v_color : COLOR0;
#endif
  
};



VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
	float3 iPos  = In.a_position;

#ifdef SKIN
	iPos  =  SkinPos(In.a_position,In.a_blendIndices,In.a_blendWeights);
#endif
   
    Out.v_position = mul(float4(iPos,1),g_matWorldViewProj); 
    
#ifdef DIFFUSE      
    Out.v_texCoord = In.a_texCoord0;
#endif

#ifdef DeferredLight
   Out.v_defTc = Out.v_position.xy / Out.v_position.w * 0.5f;
   Out.v_defTc.y *= -1;
   Out.v_defTc += 0.5f;
#endif   
    
#ifdef COLOR    
    Out.v_color = In.a_color0;
#endif
    
    return Out;
}


