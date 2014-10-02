#include "skinning.vert"


uniform float4 depth_near_far_invfar;

uniform float4x4 u_worldViewProjectionMatrix;
uniform float4x4 u_worldViewMatrix;


// Attributes
struct VS_IN
{
   float3 a_position : POSITION;

#ifdef SKIN   
   float4 a_blendWeights :BLENDWEIGHT;      
   int4   a_blendIndices :BLENDINDICES;
#endif   
 
   float3 a_normal	 : NORMAL;
   //float2 a_texcoord : TEXCOORD0;
};

struct VS_OUT
{      
   float4 oPos : POSITION;
     float4 oPos2 : TEXCOORD0;
     //float2 oTex : TEXCOORD1;
     float3 oNormal : TEXCOORD2;   
};


VS_OUT main(VS_IN In)
{
   VS_OUT vout = (VS_OUT)0;
   
   float3 wPos = In.a_position;
   float3 wNormal = In.a_normal;  
   
#ifdef SKIN
   SkinPos(In.a_position,In.a_blendWeights,In.a_blendIndices,wPos);
   SkinNormal(In.a_normal,In.a_blendWeights,In.a_blendIndices,wNormal);
#endif

   vout.oPos = mul( float4(wPos.xyz,1.0f), u_worldViewProjectionMatrix  );
   vout.oNormal = mul(float4(wNormal.xyz,0), u_worldViewMatrix).xyz;
   vout.oPos2 = float4(0,0,vout.oPos.w * depth_near_far_invfar.z,0);
   //vout.oTex = In.a_texcoord;
      
   return vout;
}


