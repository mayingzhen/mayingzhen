
#include"common.h"

#if USING_SHADOW != 0
#include"shadowmap_vs.h"
#endif

cbuffer ObjectVS : register(b5)
{
	float2 uCellAmount;
	float2 uDetailScale;
	float4 uDetailOffSet;
	float4 uDetailRotate;
	float uCurMaterialID;
	
	float3 pos_extent;
	float3 pos_center;
	float4 tc_extent_center;
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

VS_OUTPUT main(const VS_INPUT In)
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
	GetShadowPos(Out.WorldPos.xyz,Out.oShadowPos);
	#if SHADOW_BLUR == 2 	
	GetRandDirTC(Out.oPos.w,Out.oRandDirTC);  
	#endif	
#endif

    return Out;
}




