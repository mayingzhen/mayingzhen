// Uniforms
uniform float4x4 u_worldViewProjectionMatrix;

#ifdef SKIN
uniform float4x4 u_matrixPalette[SKIN_MATRIX_COUNT] : WORLDMATRIXARRAY;
#endif


// Attributes
struct VS_IN
{
   float3 a_position : POSITION;

#ifdef SKIN   
   float4 a_blendWeights :BLENDWEIGHT;      
   int4   a_blendIndices :BLENDINDICES;
#endif   
};


#ifdef SKIN
void SkinPos(float3 pos,
           float4 BlendWeights , 
           int4 BlendIndices,
           out float3 wPos)
{
   wPos = 0;
   int   IndexArray[4]   = (int[4])BlendIndices; 
   float WeightArray[4] = (float[4])BlendWeights;
   for (int iBone = 0; iBone < 4; ++iBone)
   {
      wPos += mul( u_matrixPalette[IndexArray[iBone]], float4(pos,1) ).xyz * WeightArray[iBone];
   }
}
#endif


void main( float4 Pos : POSITION,
				 float4 BlendWeights :BLENDWEIGHT, 
				 float4 BlendIndices :BLENDINDICES,	
				 out float4 oPos : POSITION )
{

   float3 wPos = Pos;

#ifdef SKIN
   SkinPos(In.a_position,In.a_blendWeights,In.a_blendIndices,wPos);
#endif

	oPos = mul( u_worldViewProjectionMatrix, float4(wPos.xyz,1.0f) );
}







