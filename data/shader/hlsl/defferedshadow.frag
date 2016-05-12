#include "common.h"
#include "depth.h"
#include "shadowmap.h"


struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
};


void main( VS_OUT In, float4 WPos : SV_Position , out float4 color : SV_TARGET )
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






	


