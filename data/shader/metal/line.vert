// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include <metal_graphics>
#include <metal_texture>
#include <metal_matrix>
#include <metal_common>
using namespace metal;

#include "common.h"


struct ObjectVS 
{
	float4x4 g_matWorldView;
	float4x4 g_matWorldViewProj;
	float4x4 g_matWorld;
}


// Attributes
struct VS_IN
{
   float3 a_position [[attribute(POSITION)]];

   float4 a_color0 : [[attribute(COLOR0)]];
};


// Varyings
struct VS_OUT
{
   float4 v_color : TEXCOORD0;
   
   float4 v_position : [[position]];

};


vertex VS_OUT main( constant ObjectVS& constants [[buffer(0)]],
    VS_IN In [[stage_in]] )
{
    VS_OUT Out = (VS_OUT)0;
  
    Out.v_position = mul(float4(In.a_position.xyz,1.0),constants.g_matWorldViewProj); 
	 
    Out.v_color = In.a_color0;
    
    return Out;
}




