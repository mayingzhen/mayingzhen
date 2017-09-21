#include "common.h"
#include "lighting.h"

#if USING_SHADOW != 0
#include "shadowMap_ps.h"
#endif

#ifdef DEFERREDSHADING 
#include "gbuffer.h"
#endif

#define RECEIVESHADOW

cbuffer ObjectPS : register(b5)
{
	float2 uBlendingOffset;
	//float4 u_cSpecColor;
	//float u_roughness;
	float g_heightScale;
};

Texture2D tBlendingMap : register(t2);
SamplerState sBlendingMap : register(s2);

Texture2D tDetailMap0 : register(t0);
SamplerState sDetailMap0 : register(s0);

Texture2D tDetailMap1 : register(t1);
SamplerState sDetailMap1 : register(s1);

Texture2D tBumpMap0 : register(t3);
SamplerState sBumpMap0 : register(s3);

Texture2D tBumpMap1 : register(t4);
SamplerState sBumpMap1 : register(s4);

struct VS_OUTPUT
{
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;

	float4 WorldPos : TEXCOORD2;
	
	float4 oNormal	: TEXCOORD3;
#ifdef BUMPMAP
	float3 oT		: TEXCOORD4; 
	float3 oB		: TEXCOORD5;  	
#endif

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
#if SHADOW_BLUR == 2
	float2 oRandDirTC : TEXCOORD6;
#endif	
	float4 oShadowPos : TEXCOORD7;
#endif
};



void main(VS_OUTPUT In,
out DRMRTOut mrtOut
)
{
	InitMRTOutPut(mrtOut);
	
	float4 oColor = 0;

	float4 albedo = 0;
	
	oColor.a = In.oNormal.w;	
	
	float4 cBlend = tBlendingMap.Sample(sBlendingMap, In.UV + uBlendingOffset);
	
#ifdef BUMPMAP
	float3x3 obj_to_tangent;
	obj_to_tangent[0] = normalize(In.oT);
	obj_to_tangent[1] = normalize(In.oB);
	obj_to_tangent[2] = normalize(In.oNormal.xyz);
#endif
		
    // 细节图
#if LAYER==1
	#ifdef PARALLAXMAPPING 
		float3 toEye = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
        float height = tBumpMap0.Sample(sBumpMap0, In.DetailUV.xy).a;		//从alpha分量得到高度信息  
        height = (height - 1.f) * g_heightScale;                 //高度倍增（向内）  
        float3x3 W2T = transpose(obj_to_tangent);  
        float3 toEyeTangent = mul(toEye, W2T);                          //世界 -> 切线空间  
        float2 offset = toEyeTangent.xy * height;                       //通过世界空间内坐标的offset获取纹理offset   
        In.DetailUV.xy += offset;                                              //纹理坐标偏移  
	#endif
	
	#ifdef BUMPMAP
		float3 Normal = tBumpMap0.Sample(sBumpMap0, In.DetailUV.xy).xyz;
	#endif	

 	float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
	albedo.rgb = cDetailMap0;
	
#elif LAYER==2
    float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
    float4 cDetailMap1 = tDetailMap1.Sample(sDetailMap1, In.DetailUV.zw);
    albedo.rgb = cDetailMap0 * cBlend.a + cDetailMap1 * (1.0 - cBlend.a);
#endif
	
#ifdef BUMPMAP
    Normal = normalize(2.0 * Normal - 1.0);
	float3 oWorldNormal = normalize( mul(Normal,obj_to_tangent) );
#else
	float3 oWorldNormal = normalize(In.oNormal.xyz);
#endif
	
	float3 vNormal = oWorldNormal;
	float3 vView  = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);

		// 阴影
	float fShadowMapShadow = 1.0;
#if USING_SHADOW != 0  && USING_DEFERREDSHADOW == 0
	#ifdef RECEIVESHADOW
		float4 ShadowPos = In.oShadowPos;
		float2 RandDirTC = 0;
			#if SHADOW_BLUR == 2
				RandDirTC = In.oRandDirTC;
			#endif
		fShadowMapShadow = DoShadowMapping(ShadowPos,RandDirTC,In.WorldPos.w);
	#endif		
#endif
	    
	float metalness = 0;
	float glossiness = 0;	
	GetMetalnessGlossiness(In.DetailUV.xy,metalness,glossiness,In.DetailUV.zw,cBlend.a);

#if DEFERREDSHADING == 0
	mrtOut.oColor.rgb = ForwardPixelLighting(metalness,glossiness,vNormal,vView,albedo.rgb,fShadowMapShadow);
#else
	FinalMRTOutPut(metalness,glossiness,albedo.rgb,vNormal,mrtOut);	
#endif
	
}



