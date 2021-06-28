#include "common.h"
#include "depth.h"
#include "shadowmap.h"


cbuffer ObjectPS : register(b5)
{
	float4 BlendInfo;

	float4x4 g_matViewToShadow;

	float4x4 g_matNextViewToShadow;

	float4 uv_calmp;

	int cascad_index;
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
	float4 pos : SV_POSITION;
	float2 oTc : TEXCOORD0;
	float4 oScreen : TEXCOORD1;
};

void vs_main(
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
    out VS_OUT vOut)
{
   vOut.pos = float4(pos.x, pos.y, 0.0, 1);
   vOut.oScreen = vOut.pos;
   vOut.oTc = iUV;
}


void ps_main( VS_OUT In, out float4 color : SV_TARGET )
{
	float depth = tDeviceDepthMapSampler.Sample(sDeviceDepthMapSampler, In.oTc).r;
	float4 position_ndc = float4(In.oScreen.x, In.oScreen.y, depth, 1.0f);
	float4 pos_es = mul(position_ndc,g_matProjInv);
	pos_es = pos_es / pos_es.w;

	float4 vShadowPos = mul(pos_es,g_matViewToShadow);

	vShadowPos = vShadowPos / vShadowPos.w;


	if (cascad_index == 0)
		color = float4(1.0,0.0,0.0,1.0);
	else if (cascad_index == 1)
		color = float4(0.0,1.0,0.0,1.0);
	else if (cascad_index == 2)
		color = float4(0.0,0.0,1.0,1.0);
	else if (cascad_index == 3)
		color = float4(1.0,1.0,0.0,1.0);
	else
		color = float4(1.0,1.0,1.0,1.0);

    float shadow = DoShadowMapping(vShadowPos,float2(0,0),vShadowPos.w);   

    //color = float4(shadow,shadow,shadow,shadow);   
	color.a = shadow; 
}






	


