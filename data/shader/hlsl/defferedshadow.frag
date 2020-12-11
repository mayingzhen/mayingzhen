#include "common.h"
#include "depth.h"
#include "shadowmap.h"


struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
};

cbuffer ObjectPS : register(b5)
{
	float4 BlendInfo;

	float4x4 g_matViewToShadow;

	float4x4 g_matNextViewToShadow;

	float4 uv_calmp;
};


float BlendVP(float3 vp, bool blendOut)
{
	float3 vBlend;
	float2 BlendVal = (blendOut) ? BlendInfo.xy : BlendInfo.zw;

	vBlend.xy =  saturate( abs(vp.xy * 2 - 1) );

	vBlend.xy -= BlendVal.x;
	vBlend.xy *= BlendVal.y;

	float fBlendValZ = 0.975;  // Magic number

	vBlend.z = vp.z * g_vCameraNearFar.z;/*fOneDivFarDist.x*/
	vBlend.z -= fBlendValZ;
	vBlend.z *=  1.0f / (1.0f - fBlendValZ);

	float result = saturate(max(max(vBlend.x,vBlend.y),vBlend.z));

	return (blendOut) ? (1.0 - result) : result;
}


void ps_main( VS_OUT In, float4 WPos : SV_Position , out float4 color : SV_TARGET )
{
	//float fLinearDepth = GetLinearDepth(In.oTc);
	//float4 vTempPos = CalcHomogeneousPosDepth(WPos.xy,fLinearDepth);
	//float depth = vTempPos.w; 
	//float4 vShadowPos = float4(vTempPos.xyz,1.0);
	float fLinearDepth = GetLinearDepth(In.oTc); 
	float3 view_dir = normalize(In.oViewDir.xyz);
	float3 pos_es = view_dir * (fLinearDepth / view_dir.z); 
	float4 vShadowPos = mul(float4(pos_es,1.0),g_matViewToShadow);

	//vShadowPos.xy = clamp(vShadowPos.xy,uv_calmp.xy,uv_calmp.zw);
		
   	float2 RandDirTC;    
#if SHADOW_BLUR == 2
	GetRandDirTC(depth,RandDirTC);	
#endif
   
    float shadow = DoShadowMapping(vShadowPos,RandDirTC,vShadowPos.w);   

#ifdef FRUSTUM_BLEND 
	float fBlend = BlendVP(vShadowPos.xyz, true);
	
	//float4 vTempNextPos = CalcHomogeneousNextPos(WPos.xy,fLinearDepth);
	//float Nextdepth = vTempNextPos.w; 
	//float4 vNextShadowPos = float4(vTempNextPos.xyz,1.0);
	float4 vNextShadowPos = mul(float4(pos_es,1.0),g_matNextViewToShadow);
		
   	float2 NextRandDirTC;    
#if SHADOW_BLUR == 2
	GetRandDirTC(Nextdepth,NextRandDirTC);	
#endif
   
    float nextshadow = DoShadowMapping(vNextShadowPos,NextRandDirTC,vNextShadowPos.w);  

	shadow = lerp(nextshadow,shadow,fBlend);

#endif

    color = float4(shadow,shadow,shadow,1.0);    
}






	


