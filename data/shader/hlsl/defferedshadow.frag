#include "common.h"
#include "depth.h"
#include "shadowmap.h"


struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
};


void main( float4 WPos : VPOS, VS_OUT In,out float4 color : COLOR0 )
{
	float4 vTempPos = CalcHomogeneousPos(In.oTc, WPos.xy);
	float depth = vTempPos.w; 
	float4 vShadowPos = float4(vTempPos.xyz,1.0);
		
   	float2 RandDirTC;    
#if SHADOW_BLUR == 2
	GetRandDirTC(depth,RandDirTC);	
#endif
   
   float shadow = DoShadowMapping(vShadowPos,RandDirTC,depth);   
   
   color = shadow;
}






	


