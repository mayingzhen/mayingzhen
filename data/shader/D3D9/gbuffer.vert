uniform float4 depth_near_far_invfar;

uniform float4x4 u_worldViewProjectionMatrix;
uniform float4x4 u_worldViewMatrix;

#ifdef SKIN
uniform float4x4 u_matrixPalette[SKIN_MATRIX_COUNT] : WORLDMATRIXARRAY;
#endif



struct VS_OUT
{      
   float4 oPos : POSITION;
     float4 oPos2 : TEXCOORD0;
     float2 oTex : TEXCOORD1;
     float3 oNormal : TEXCOORD2;   
};


#ifdef SKIN
void SkinPos(float3 pos,
           float4 a_blendWeights , 
           int4 	a_blendIndices,
           out float3 wPos)
{
    wPos = 0;
	wPos += mul(u_matrixPalette[a_blendIndices.x], float4(pos,1.0)).xyz * a_blendWeights.x;
	wPos += mul(u_matrixPalette[a_blendIndices.y], float4(pos,1.0)).xyz * a_blendWeights.y;
	wPos += mul(u_matrixPalette[a_blendIndices.z], float4(pos,1.0)).xyz * a_blendWeights.z;
	wPos += mul(u_matrixPalette[a_blendIndices.w], float4(pos,1.0)).xyz * a_blendWeights.w;  
}

void SkinNormal(float3 normal,
           float4 a_blendWeights , 
           int4 	a_blendIndices,
           out float3 wNormal)
{
	wNormal = 0;

	wNormal += mul(u_matrixPalette[a_blendIndices.x], float4(normal,0)).xyz * a_blendWeights.x;
	wNormal += mul(u_matrixPalette[a_blendIndices.y], float4(normal,0)).xyz * a_blendWeights.y;
	wNormal += mul(u_matrixPalette[a_blendIndices.z], float4(normal,0)).xyz * a_blendWeights.z;
	wNormal += mul(u_matrixPalette[a_blendIndices.w], float4(normal,0)).xyz * a_blendWeights.w;
}
#endif


VS_OUT main( float3 pos : POSITION,
                 float4 BlendWeights :BLENDWEIGHT, 
                  float4 BlendIndices :BLENDINDICES,
                  float3 normal : NORMAL,
                      float2 texcoord : TEXCOORD0 )
{
   VS_OUT vout = (VS_OUT)0;
   
   float3 wPos = pos;
   float3 wNormal = normal;  
   
#ifdef SKIN
   SkinPos(pos,BlendWeights,BlendIndices,wPos);
   SkinNormal(normal,BlendWeights,BlendIndices,wNormal);
#endif

   vout.oPos = mul( u_worldViewProjectionMatrix, float4(wPos.xyz,1.0f) );
   vout.oNormal = mul(u_worldViewMatrix, float4(wNormal.xyz,0)).xyz;
   vout.oPos2 = float4(0,0,vout.oPos.w * depth_near_far_invfar.z,0);
   vout.oTex = texcoord;
      
   return vout;
}


