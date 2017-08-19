// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------
using namespace metal;

#define BONE_DQ

/*
#ifndef MAX_DQ_NUM_BONES
#define MAX_DQ_NUM_BONES 100
#endif
#ifndef MAX_MAT_NUM_BONES
#define MAX_MAT_NUM_BONES 75
#endif

#define BONE_DQ

#ifdef BONE_DQ
//float4 boneDQ[MAX_DQ_NUM_BONES*2];
#else
//float4x3 boneMatrix[MAX_MAT_NUM_BONES];
#endif
*/
 
#ifdef BONE_DQ
static float2x4 GetblendDQ(uint4 iBoneIndex, float4 iBoneWeight,constant float4* boneDQ)
{
	iBoneIndex = min(iBoneIndex,uint4(MAX_MAT_NUM_BONES - 1)); 
	uint4 idx = iBoneIndex * 2;
	float2x4 dq0 = float2x4(boneDQ[idx.x], boneDQ[idx.x + 1]);
	float2x4 dq1 = float2x4(boneDQ[idx.y], boneDQ[idx.y + 1]);
	float2x4 dq2 = float2x4(boneDQ[idx.z], boneDQ[idx.z + 1]);
	float2x4 dq3 = float2x4(boneDQ[idx.w], boneDQ[idx.w + 1]);
	dq0 *= sign(dot(dq0[0], dq0[0]));
	dq1 *= sign(dot(dq0[0], dq1[0]));
	dq2 *= sign(dot(dq0[0], dq2[0]));
	dq3 *= sign(dot(dq0[0], dq3[0]));

#if BONE==1
	float2x4 blendDQ = dq0 * iBoneWeight.x;
#elif BONE==2
	float2x4 blendDQ = dq0 * iBoneWeight.x;
	blendDQ += dq1 * iBoneWeight.y;
#elif BONE==3
	float2x4 blendDQ = dq0 * iBoneWeight.x;
	blendDQ += dq1 * iBoneWeight.y;
	blendDQ += dq2 * iBoneWeight.z;
#else
	float2x4 blendDQ = dq0 * iBoneWeight.x;
	blendDQ += dq1 * iBoneWeight.y;
	blendDQ += dq2 * iBoneWeight.z;
	blendDQ += dq3 * iBoneWeight.w;        
#endif

	float len = length(blendDQ[0]);
	blendDQ[0] = blendDQ[0] / len;
	blendDQ[1] = blendDQ[1] / len;

	return blendDQ;
}

static float3 DQSkinPos(float3 iPos,float2x4 blendDQ)
{
	float3 finalPos = iPos.xyz + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iPos.xyz) + blendDQ[0].x*iPos.xyz);
	float3 trans = 2.0*(blendDQ[0].x*blendDQ[1].yzw - blendDQ[1].x*blendDQ[0].yzw + cross(blendDQ[0].yzw, blendDQ[1].yzw));
	finalPos += trans;

	return finalPos;
}

#else

static float4x4 GetSkinMatrix(float4 blendWeights, uint4 blendIndices,constant float4x3* boneMatrix)
{
    float4x3 skinMatrix;
#if BONE==1
	skinMatrix = boneMatrix[blendIndices.x] * blendWeights.x;
#elif BONE==2
	skinMatrix = boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y ;
#elif BONE==3
	skinMatrix = boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y +
		boneMatrix[blendIndices.z] * blendWeights.z;
#else
	skinMatrix = boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y +
		boneMatrix[blendIndices.z] * blendWeights.z +
		boneMatrix[blendIndices.w] * blendWeights.w;
#endif
    return float4x4( float4(skinMatrix[0],0),
                    float4(skinMatrix[1],0),
                    float4(skinMatrix[2],0),
                    float4(skinMatrix[3],1));
}

#endif


static float3 SkinPos(float3 iPos,uint4 iBoneIndex, float4 iBoneWeight,
                    #ifdef BONE_DQ
                      constant float4* boneDQ
                    #else
                     constant float4x3* boneMatrix
                    #endif
                      )
{
#ifdef BONE_DQ
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight,boneDQ);

	float3 finalPos = DQSkinPos(iPos,blendDQ);

#else
	float3 finalPos = (float4(iPos,1.0) * GetSkinMatrix(iBoneWeight,iBoneIndex,boneMatrix)).xyz;

#endif

	return finalPos;
}
struct SkinOutput
{
    float3 pos;
    float3 nor;
};

static SkinOutput SkinPosNormal(float3 iPos,float3 iNormal,uint4 iBoneIndex, float4 iBoneWeight,
                        #ifdef BONE_DQ
                          constant float4* boneDQ
                        #else
                          constant float4x3* boneMatrix
                        #endif
                          )
{
    float3 finalPos;
    float3 finalNormal;
    
#ifdef BONE_DQ
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight,boneDQ);

	finalPos = DQSkinPos(iPos,blendDQ);

	finalNormal = iNormal + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iNormal) + blendDQ[0].x*iNormal);

#else
	float4x4 skinMatrix = GetSkinMatrix(iBoneWeight,iBoneIndex,boneMatrix);
	finalPos = (float4(iPos,1.0) * skinMatrix).xyz;
	finalNormal = (float4(iNormal,0.0) * skinMatrix).xyz;
#endif
    
    SkinOutput skin;
    skin.pos = finalPos;
    skin.nor = finalNormal;
    return skin;

}
