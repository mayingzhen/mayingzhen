// #define SKIN
// #define DEFERREDSHADING
// #define DIFFUSE

#include "common.h"

#ifdef SKIN

#define BONE_DQ

#ifndef MAX_DQ_NUM_BONES
#define MAX_DQ_NUM_BONES 100
#endif
#ifndef MAX_MAT_NUM_BONES
#define MAX_MAT_NUM_BONES 75
#endif

#endif

struct ObjectVS
{
    float4x4 g_matWorldView;
    float4x4 g_matWorldViewProj;
    float4x4 g_matWorld;
    
    float3 pos_extent;
    float3 pos_center;
    float4 tc_extent_center;

#ifdef SKIN
    #ifdef BONE_DQ
    float4 boneDQ[MAX_DQ_NUM_BONES*2];
    #else
    float4x3 boneMatrix[MAX_MAT_NUM_BONES];
    #endif
#endif
};

#ifdef SKIN
#include "skin.h"
#endif


// Attributes
struct VS_IN
{
    float3 a_position [[attribute(0)]];
    
    float4 a_normal	 [[attribute(1)]];
    
    float2 a_texCoord0 [[attribute(2)]];
    
#ifdef SKIN
    uint4   a_blendIndices [[attribute(3)]];
    float4 a_blendWeights [[attribute(4)]];
#endif
    
    //float3 a_tangent  : TANGENT;
    
};


// Varyings
struct VS_OUT
{
    float4 v_position [[position]];
    
    float2 v_texCoord;
    
    float4 WorldPos ;
    float4 worldNormal ;
    
#ifdef DEFERREDSHADING
    float3 viewNormal ;
#else
    
#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
    float2 oRandDirTC ;
    float4 oShadowPos ;
#endif
#endif
    
#ifdef COLOR
    float4 v_color ;
#endif
    
};


vertex VS_OUT vs_main(VS_IN In [[stage_in]],
                      constant ObjectVS& ov [[buffer(1)]],
                      constant CameraVS& cv [[buffer(2)]] )
{
    VS_OUT Out;
    
    float3 iPos = (In.a_position * ov.pos_extent + ov.pos_center);
    float2 iUV  = In.a_texCoord0 * ov.tc_extent_center.xy + ov.tc_extent_center.zw;
    float3 iNormal = In.a_normal.xyz * 2.0 - 1.0;
    

#ifdef SKIN
    SkinOutput skin;
    #ifdef BONE_DQ
    skin = SkinPosNormal(iPos,iNormal,In.a_blendIndices,In.a_blendWeights,ov.boneDQ);
    #else
    skin = SkinPosNormal(iPos,iNormal,In.a_blendIndices,In.a_blendWeights,ov.boneMatrix);
    #endif
    iPos  = skin.pos;
    iNormal = skin.nor;
#endif

    
    float3 WorldPos = (float4(iPos,1.0) * ov.g_matWorld).xyz;
    
    Out.v_position = float4(WorldPos,1.0) * cv.g_matViewProj;
    
    Out.WorldPos.xyz = WorldPos;
    Out.WorldPos.w = Out.v_position.w;
    
    Out.worldNormal.xyz = (float4(iNormal,0.0) * ov.g_matWorld).xyz;
    Out.worldNormal.w = 0;
    
    Out.v_texCoord = iUV;
    
    
#ifdef COLOR
    Out.v_color = In.a_color0;
#endif
    
#ifdef DEFERREDSHADING
    Out.viewNormal.xyz = mul(iNormal, (float3x3)g_matWorldView);
#endif
    
#if USING_SHADOW != 0 && USING_DEFERREDSHADOW == 0
    GetShadowPos(Out.WorldPos.xyz,Out.oShadowPos);
#if SHADOW_BLUR == 2 	
    GetRandDirTC(Out.oPos.w,Out.oRandDirTC);
#endif	
#endif
    
    return Out;
}









