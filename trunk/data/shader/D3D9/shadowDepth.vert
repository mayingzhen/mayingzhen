// Uniforms
uniform float4x4 u_worldViewProjectionMatrix;

#ifndef HWPCF
uniform float4x4 u_worldViewMatrix; 
#endif

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

struct VS_OUT
{
	float4 oPos : POSITION;
	
#ifndef HWPCF
	float2 oDepth : TEXCOORD0;
#endif	
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
#endif


VS_OUT main( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
 
   float3 wPos = In.a_position;
   
#ifdef SKIN
   SkinPos(In.a_position,In.a_blendWeights,In.a_blendIndices,wPos);
#endif

	Out.oPos = mul( u_worldViewProjectionMatrix, float4(wPos,1.0f) );

#ifndef HWPCF
	Out.oDepth = Out.oPos.zw;  
#endif	

	return Out;  
}








