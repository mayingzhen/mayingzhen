#include "skin.h"


// Uniforms
uniform float4x4 u_worldViewProjectionMatrix;

//#ifndef HWPCF
//uniform float4x4 u_worldViewMatrix; 
//#endif


// Attributes
struct VS_IN
{
   float3 a_position : POSITION;

#ifdef SKIN   
   float4 a_blendWeights :BLENDWEIGHT;      
   int4   a_blendIndices :BLENDINDICES;
#endif   
};

struct VS_OUT
{
	float4 oPos : POSITION;
	
#ifndef HWPCF
	float4 oDepth : TEXCOORD0;
#endif	
};




VS_OUT main( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
 
	float3 iPos  = In.a_position;

#ifdef SKIN
	iPos  =  SkinPos(In.a_position,In.a_blendIndices,In.a_blendWeights);
#endif

	Out.oPos = mul( float4(iPos,1.0f), u_worldViewProjectionMatrix);

#ifndef HWPCF
	Out.oDepth = Out.oPos;  
#endif	

	return Out;  
}








