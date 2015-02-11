
// #define DEFERREDSHADING
// #define USING_SHADOW 1 

#include"common.h"

#if USING_SHADOW != 0
#include"shadowMap.h"
#endif

#ifdef DEFERREDSHADING 
#include"gbuffer.h"
#endif 

uniform float2 uCellAmount;
uniform float2 uDetailScale;
uniform float4 uDetailOffSet;
uniform float4 uDetailRotate;
uniform float uCurMaterialID;


struct VS_INPUT
{
	float3	Pos		: POSITION;
	float2  UV      : TEXCOORD0;
    float3  Normal  : NORMAL;
    float3  Tan     : TANGENT;
	float4  Color	: COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;
	float4 Color	: TEXCOORD2;
	float4 WorldPos : TEXCOORD3;
#ifdef DEFERREDSHADING 
   float4 v_normalDepth  :TEXCOORD4;	
#endif  
#if USING_SHADOW != 0
	float2 RandDirTC : TEXCOORD5;
	float4 ShadowPos[g_iNumSplits] : TEXCOORD6;
#endif	
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

VS_OUTPUT main(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.WorldPos = float4(v.Pos, 1.0);
	Out.Pos = mul(Out.WorldPos,g_matViewProj);
	Out.WorldPos.w = Out.Pos.w;

    Out.UV = v.UV;
    
#if USING_SHADOW != 0
	GetShadowPos(Out.WorldPos.xyz,Out.WorldPos.w,Out.ShadowPos,Out.RandDirTC);
#endif

#if LAYER==1
	Out.DetailUV.xy = GetDetaiUV( v.UV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
#elif LAYER==2
    Out.DetailUV.xy = GetDetaiUV( v.UV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
    Out.DetailUV.zw = GetDetaiUV( v.UV * uCellAmount, uDetailScale.y, uDetailRotate.zw, uDetailOffSet.zw );
#endif

	Out.Color = v.Color;
	float fWeight = saturate(1 - abs(v.Color.a * 255.0 - uCurMaterialID));	
	Out.Color.a = fWeight;
	
#ifdef DEFERREDSHADING 
	GBufferVSOut(v.Normal.xyz,Out.Pos.w,Out.v_normalDepth);
#endif   

    return Out;
}




