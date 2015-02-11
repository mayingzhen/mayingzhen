#include "common.h"
#include "shadowmap.h"

// Gbuffer
sampler2D u_textureSceneDepth;


struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};


float GetLinearDepth(float2 tc)
{
#ifdef HWDEPTH
	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = tex2D(u_textureSceneDepth, tc).r;
	depth = g_vCameraNearFar.x / (q - depth);
#else
	float depth = tex2D(u_textureSceneDepth, tc).r;
    depth *= g_vCameraNearFar.y;
#endif	
	
	return depth;
}

void main( VS_OUT In,out float4 color : COLOR0 )
{
	float depth = GetLinearDepth(In.oTc); 
	
   float3 view_dir = normalize(In.oViewDir);
   float3 pos_es = view_dir * (-depth / view_dir.z); 
   
   float3 worldPos = mul(float4(pos_es,1.0),g_matViewInv);  
   
   	float2 RandDirTC;
	float4 ShadowPos[g_iNumSplits];
	
    GetShadowPos(worldPos.xyz,depth,ShadowPos,RandDirTC);
   
   float shadow = DoShadowMapping(ShadowPos,RandDirTC,depth);   
   
   color = shadow;
}






	


