#include "common.h"
#include "lighting.h"

#if USING_SHADOW != 0
#include "shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include "gbuffer.h"
#endif

#define RECEIVESHADOW

cbuffer ObjectVS : register(b5)
{
	float2 uCellAmount;
	float2 uDetailScale;
	float4 uDetailOffSet;
	float4 uDetailRotate;
	
	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;

	float uCurMaterialID;
};


struct VS_INPUT
{
	float4	Pos		: POSITION;
	float2  UV      : TEXCOORD0;
    float4  TanQuat : TANGENT;
};

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

	float4 Pos		: SV_POSITION;
};


float2 GetDetaiUV(float2 oUVPixel,float fDetailScale, float2 fDetailRot, float2 uDetailOffSet)
{
	float2 uv = oUVPixel * fDetailScale;

#ifdef DETAILROTATE
	float rsin = fDetailRot.x;
    float rcos = fDetailRot.y;
    float2x2 rotateP = { rcos, -rsin, rsin, rcos };
  
	uv = mul(uv, rotateP);
#endif

	uv += uDetailOffSet;

	return uv;
}

VS_OUTPUT vs_main(const VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
	float3 iPos = In.Pos.xyz * pos_extent + pos_center;	
	float2 iUV  = In.UV * tc_extent_center.xy + tc_extent_center.zw;
	float4 iTanQuat = In.TanQuat * 2.0 - 1.0; 
	float  iMateriaID = In.Pos.w * 32767.5f;

	Out.WorldPos = float4(iPos, 1.0);
	Out.Pos = mul(Out.WorldPos,g_matViewProj);
	Out.WorldPos.w = Out.Pos.w;

    Out.UV = iUV;
		
#if defined(BUMPMAP)
	Out.oT = transform_quat(float3(1, 0, 0), iTanQuat);
	Out.oB = transform_quat(float3(0, 1, 0), iTanQuat) * sign(iTanQuat.w);	
#endif
	Out.oNormal.xyz = transform_quat(float3(0, 0, 1), iTanQuat);


#if LAYER==1
	Out.DetailUV.xy = GetDetaiUV( iUV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
#elif LAYER==2
    Out.DetailUV.xy = GetDetaiUV( iUV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
    Out.DetailUV.zw = GetDetaiUV( iUV * uCellAmount, uDetailScale.y, uDetailRotate.zw, uDetailOffSet.zw );
#endif

	float fWeight = saturate(1 - abs(iMateriaID - uCurMaterialID));	
	Out.oNormal.w = fWeight;

#if USING_SHADOW != 0  && USING_DEFERREDSHADOW == 0
	Out.oShadowPos = GetShadowPos(Out.WorldPos.xyz);
	#if SHADOW_BLUR == 2 	
		GetRandDirTC(Out.oPos.w,Out.oRandDirTC);  
	#endif	
#endif

    return Out;
}


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


void ps_main(VS_OUTPUT In,
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
		
    // ϸ��ͼ
#if LAYER==1
	#ifdef PARALLAXMAPPING 
		float3 toEye = normalize(g_vEyeWorldPos.xyz - In.WorldPos.xyz);
        float height = tBumpMap0.Sample(sBumpMap0, In.DetailUV.xy).a;		//��alpha�����õ��߶���Ϣ  
        height = (height - 1.f) * g_heightScale;                 //�߶ȱ��������ڣ�  
        float3x3 W2T = transpose(obj_to_tangent);  
        float3 toEyeTangent = mul(toEye, W2T);                          //���� -> ���߿ռ�  
        float2 offset = toEyeTangent.xy * height;                       //ͨ������ռ��������offset��ȡ����offset   
        In.DetailUV.xy += offset;                                              //��������ƫ��  
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

		// ��Ӱ
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
	mrtOut.oColor.rgb = ForwardPixelLighting(metalness,glossiness,vNormal,vView,albedo.rgb,fShadowMapShadow,1.0);
#else
	EnCodeMRTOutPut(metalness,glossiness,albedo.rgb,vNormal,mrtOut);	
#endif
	
}



