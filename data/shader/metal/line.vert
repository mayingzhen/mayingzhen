// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE



#include "common.h"


struct ObjectVS 
{
	float4x4 g_matWorldView;
	float4x4 g_matWorldViewProj;
	float4x4 g_matWorld;
};


// Attributes
struct VS_IN
{
    float3 a_position [[attribute(0)]];

   float4 a_color0  [[attribute(1)]];
};


// Varyings
struct VS_OUT
{
   float4 v_color ;
   
   float4 v_position [[position]];

};


vertex VS_OUT vs_main( VS_IN In [[stage_in]],
                   constant ObjectVS& constants [[buffer(1)]] )
{
    VS_OUT Out;
  
    Out.v_position = float4(In.a_position.xyz,1.0) * constants.g_matWorldViewProj;
	 
    Out.v_color = In.a_color0;
    
    return Out;
}




