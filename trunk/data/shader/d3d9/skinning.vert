
#ifdef SKIN
uniform float4x4 u_matrixPalette[55] : WORLDMATRIXARRAY;


void SkinPos(float3 pos,
           float4 a_blendWeights , 
           int4 	a_blendIndices,
           out float3 wPos)
{
    wPos = 0;
	wPos += mul(float4(pos,1.0),u_matrixPalette[a_blendIndices.x]).xyz * a_blendWeights.x;
	wPos += mul(float4(pos,1.0),u_matrixPalette[a_blendIndices.y]).xyz * a_blendWeights.y;
	wPos += mul(float4(pos,1.0),u_matrixPalette[a_blendIndices.z]).xyz * a_blendWeights.z;
	wPos += mul(float4(pos,1.0),u_matrixPalette[a_blendIndices.w]).xyz * a_blendWeights.w;  
}


void SkinNormal(float3	normal,
           float4		a_blendWeights , 
           int4 		a_blendIndices,
           out float3	wNormal)
{
	wNormal = 0;

	wNormal += mul(float4(normal,0),u_matrixPalette[a_blendIndices.x]).xyz * a_blendWeights.x;
	wNormal += mul(float4(normal,0),u_matrixPalette[a_blendIndices.y]).xyz * a_blendWeights.y;
	wNormal += mul(float4(normal,0),u_matrixPalette[a_blendIndices.z]).xyz * a_blendWeights.z;
	wNormal += mul(float4(normal,0),u_matrixPalette[a_blendIndices.w]).xyz * a_blendWeights.w;
}

#endif