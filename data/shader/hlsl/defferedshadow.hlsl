#include "common.h"
#include "depth.h"
#include "shadowmap.h"


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


struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oViewDir : TEXCOORD1;
   float4 pos : SV_POSITION;
};

void vs_main(
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
    out VS_OUT vOut)
{
   vOut.pos = float4(pos.x, pos.y, 0.0, 1);
   vOut.oViewDir = mul(float4(vOut.pos.xyz ,1),g_matProjInv);
   vOut.oTc = iUV;
}


void ps_main( VS_OUT In, float4 WPos : SV_Position , out float4 color : SV_TARGET )
{
	float fLinearDepth = GetLinearDepth(In.oTc); 
	float3 view_dir = normalize(In.oViewDir.xyz);
	float3 pos_es = view_dir * (fLinearDepth / view_dir.z); 

	float4 vShadowPos = mul(float4(pos_es,1.0),g_matViewToShadow);

    float shadow = DoShadowMapping(vShadowPos,float2(0,0),vShadowPos.w);   

    color = float4(shadow,shadow,shadow,1.0);    
}






	


