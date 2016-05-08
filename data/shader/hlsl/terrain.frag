#include "common.h"

#if USING_SHADOW != 0
#include "shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include "gbuffer.h"
#endif

cbuffer ObjectPS : register(b5)
{
	uniform float4 u_cSpecColor;

	uniform float2 uBlendingOffset;
}

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


float4 GetDiffuse(VS_OUTPUT In)
{
	float4 oColor = float4(1,1,1,1);
	
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
		float3 toEye = normalize(g_vEyeWorldPos.xyz - oWorldPos.xyz);
        float height = tex2D(tBumpMap0, oUVPixel0.xy).a;		//从alpha分量得到高度信息  
        height = (height - 1.f) * g_heightScale;                 //高度倍增（向内）  
        float3x3 W2T = transpose(obj_to_tangent);  
        float3 toEyeTangent = mul(toEye, W2T);                          //世界 -> 切线空间  
        float2 offset = toEyeTangent.xy * height;                       //通过世界空间内坐标的offset获取纹理offset   
        oUVPixel0.xy += offset;                                              //纹理坐标偏移  
	#endif
	
	#ifdef BUMPMAP
		float3 Normal = tex2D(tBumpMap0, oUVPixel0.xy).xyz;
	#endif	

 	float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
	oColor = cDetailMap0;
#elif LAYER==2
    float4 cDetailMap0 = tDetailMap0.Sample(sDetailMap0, In.DetailUV.xy);
    float4 cDetailMap1 = tDetailMap1.Sample(sDetailMap1, In.DetailUV.zw);
    oColor = cDetailMap0 * cBlend.a + cDetailMap1 * (1.0 - cBlend.a);
#endif
	
#ifdef BUMPMAP
    Normal = normalize(2.0 * Normal - 1.0);
	float3 oWorldNormal = normalize( mul(Normal,obj_to_tangent) );
#else
	float3 oWorldNormal = normalize(In.oNormal.xyz);
#endif

#if USING_DEFERREDLIGHTING == 0   
#if defined(BUMPMAP) || defined(LIGHTING)
	float3 oL = normalize(g_vDirLight);
	float fNDotL = max( dot(oWorldNormal,oL) , 0);
    cDiff = g_cSkyLight + g_cDirLight * fNDotL;
#endif
#endif

// 高光
#if USING_DEFERREDLIGHTING == 0
#ifdef SPEC 	
    float3 refl = reflect(-g_vDirLight, normalize(oWorldNormal));	
    float cosVal = dot(normalize(g_vEyeWorldPos.xyz - oWorldPos.xyz), refl);
    float3 spec = g_cDirLight* pow(max(0,cosVal), specPower) * specMaterial;
    oColor.rgb += spec * specAlpha;
#endif
#endif


	oColor.a = In.oNormal.w;		

	return oColor;
}

#ifndef DEFERREDSHADING 
float4 ForwardShading(float4 cDiffuse,VS_OUTPUT In)
{
	float4 flagColor = cDiffuse;

#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
	float4 ShadowPos = In.oShadowPos;
	float2 RandDirTC = 0;
#if SHADOW_BLUR == 2
	RandDirTC = In.oRandDirTC;
#endif
	flagColor.rgb *= DoShadowMapping(ShadowPos,RandDirTC,In.WorldPos.w);		
#endif
	
	return flagColor;
}
#endif

void main(VS_OUTPUT In,
#if defined(DEFERREDSHADING)
out PS_OUT pout
#else
out float4 outColor : SV_TARGET 
#endif
)
{
	float4 cDiffuse = GetDiffuse(In);

#if defined(DEFERREDSHADING) 
	pout = GbufferPSout(cDiffuse,u_cSpecColor,In.v_normalDepth);
#else
	outColor = ForwardShading(cDiffuse,In);
#endif	
}



