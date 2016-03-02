// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include "common.h"



// Attributes
struct VS_IN
{
   float3 a_position : POSITION;

   float4 a_color0 : COLOR0;
};


// Varyings
struct VS_OUT
{
   float4 v_position : POSITION;
     
   float4 v_color : COLOR0;

};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
  
    Out.v_position = mul(float4(In.a_position.xyz,1.0),g_matWorldViewProj); 
	 
    Out.v_color = In.a_color0;
    
    return Out;
}




