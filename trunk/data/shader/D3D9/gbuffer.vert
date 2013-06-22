uniform float4x4 worldviewprojection ;
uniform float4x4 worldview ;

#ifdef SKIN
uniform float4x4 u_matrixPalette[SKIN_MATRIX_COUNT] : WORLDMATRIXARRAY;
#endif


uniform float4 depth_near_far_invfar;


struct VS_OUT
{      
   float4 oPos : POSITION;
     float4 oPos2 : TEXCOORD0;
     float2 oTex : TEXCOORD1;
     float3 oNormal : TEXCOORD2;   
};


#ifdef SKIN
void SkinPos( float3 pos ,
               float3 normal,
             float4 BlendWeights , 
               int4 BlendIndices,
               out float3 wPos,
               out float3 wNormal)
{
   wPos = 0;
   wNormal = 0;
   int   IndexArray[4]   = (int[4])BlendIndices; 
   float WeightArray[4] = (float[4])BlendWeights;
   for (int iBone = 0; iBone < 4; ++iBone)
   {
      wPos += mul(u_matrixPalette[IndexArray[iBone]], float4(wPos.xyz, 1.0f)).xyz * WeightArray[iBone];
      wNormal += mul((float3x3)u_matrixPalette[IndexArray[iBone]], wNormal) * WeightArray[iBone];
   }
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
   SkinPos(pos,normal,BlendWeights,BlendIndices,wPos,wNormal);
#endif

      vout.oPos = mul( worldviewprojection, float4(wPos.xyz,1.0f) );
      vout.oPos2 = float4(0,0,vout.oPos.w * depth_near_far_invfar.z,0);
      vout.oTex = texcoord;
      vout.oNormal = mul((float3x3)worldview,wNormal);
      
      return vout;
}


