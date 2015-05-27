// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------

#ifndef MAX_DQ_NUM_BONES
#define MAX_DQ_NUM_BONES 100
#endif
#ifndef MAX_MAT_NUM_BONES
#define MAX_MAT_NUM_BONES 75
#endif

#define BONE_DQ

#ifdef BONE_DQ
float4 boneDQ[MAX_DQ_NUM_BONES*2];
float4 boneScale[MAX_DQ_NUM_BONES/4];
#else
float4x3 boneMatrix[MAX_MAT_NUM_BONES];
#endif

#ifdef BONE_DQ
float GetBoneScale(int4 iBoneIndex, float4 iBoneWeight)
{
#if BONE==1
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][iBoneIndex.x%4]*iBoneWeight.b;
#elif BONE==2
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][iBoneIndex.x%4]*iBoneWeight.b;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][iBoneIndex.y%4]*iBoneWeight.g;
#elif BONE==3
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][iBoneIndex.x%4]*iBoneWeight.b;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][iBoneIndex.y%4]*iBoneWeight.g;
	fBoneScale += boneScale[int(iBoneIndex.z*0.25)][iBoneIndex.z%4]*iBoneWeight.r;
#else
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][iBoneIndex.x%4]*iBoneWeight.b;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][iBoneIndex.y%4]*iBoneWeight.g;
	fBoneScale += boneScale[int(iBoneIndex.z*0.25)][iBoneIndex.z%4]*iBoneWeight.r;
	fBoneScale += boneScale[int(iBoneIndex.w*0.25)][iBoneIndex.w%4]*iBoneWeight.a;
#endif

	return fBoneScale;
}

float2x4 GetblendDQ(int4 iBoneIndex, float4 iBoneWeight)
{
	int y0 = iBoneIndex.x*2;
	int y1 = iBoneIndex.y*2;
	int y2 = iBoneIndex.z*2;
	int y3 = iBoneIndex.w*2;
	float2x4 dq0 = float2x4(boneDQ[y0], boneDQ[y0+1]);
	float2x4 dq1 = float2x4(boneDQ[y1], boneDQ[y1+1]);
	float2x4 dq2 = float2x4(boneDQ[y2], boneDQ[y2+1]);
	float2x4 dq3 = float2x4(boneDQ[y3], boneDQ[y3+1]);
	dq1 *= sign(dot(dq0[0], dq1[0]));
	dq2 *= sign(dot(dq0[0], dq2[0]));
	dq3 *= sign(dot(dq0[0], dq3[0]));

#if BONE==1
	float2x4 blendDQ = dq0*iBoneWeight.x;
#elif BONE==2
	float2x4 blendDQ = dq0*iBoneWeight.x;
	blendDQ += dq1*iBoneWeight.y;
#elif BONE==3
	float2x4 blendDQ = dq0*iBoneWeight.x;
	blendDQ += dq1*iBoneWeight.y;
	blendDQ += dq2*iBoneWeight.z;
#else
	float2x4 blendDQ = dq0*iBoneWeight.x;
	blendDQ += dq1*iBoneWeight.y;
	blendDQ += dq2*iBoneWeight.z;
	blendDQ += dq3*iBoneWeight.w;        
#endif

	blendDQ /= length(blendDQ[0]);

	return blendDQ;
}

float3 DQSkinPos(float3 iPos,float fBoneScale,float2x4 blendDQ)
{
//#ifdef BONESCALE
	iPos.xyz *= fBoneScale;
//#endif

	float3 finalPos = iPos.xyz + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iPos.xyz) + blendDQ[0].x*iPos.xyz);
	float3 trans = 2.0*(blendDQ[0].x*blendDQ[1].yzw - blendDQ[1].x*blendDQ[0].yzw + cross(blendDQ[0].yzw, blendDQ[1].yzw));
	finalPos += trans;

	return finalPos;
}

#else

float4x3 GetSkinMatrix(float4 blendWeights, int4 blendIndices)
{
#if BONE==1
	return boneMatrix[blendIndices.x] * blendWeights.x;
#elif BONE==2
	return boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y ;
#elif BONE==3
	return boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y +
		boneMatrix[blendIndices.z] * blendWeights.z;
#else
	return boneMatrix[blendIndices.x] * blendWeights.x +
		boneMatrix[blendIndices.y] * blendWeights.y +
		boneMatrix[blendIndices.z] * blendWeights.z +
		boneMatrix[blendIndices.w] * blendWeights.w;
#endif
}

#endif

float3 SkinPos(float3 iPos,int4 iBoneIndex, float4 iBoneWeight)
{
#ifdef BONE_DQ
	float fBoneScale = GetBoneScale(iBoneIndex,iBoneWeight);
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight);

	float3 finalPos = DQSkinPos(iPos,fBoneScale,blendDQ);

#else
	float3 finalPos = mul(float4(iPos,1.0),GetSkinMatrix(iBoneWeight,iBoneIndex)).xyz;

#endif

	return finalPos;
}

void SkinPosNormal(float3 iPos,float3 iNormal,int4 iBoneIndex, float4 iBoneWeight, out float3 finalPos, out float3 finalNormal)
{
#ifdef BONE_DQ
	float fBoneScale = GetBoneScale(iBoneIndex,iBoneWeight);
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight);

	finalPos = DQSkinPos(iPos,fBoneScale,blendDQ);

	finalNormal = iNormal + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iNormal) + blendDQ[0].x*iNormal);

#else
	float4x3 skinMatrix = GetSkinMatrix(iBoneWeight,iBoneIndex);
	finalPos = mul(float4(iPos,1.0),skinMatrix).xyz;
	finalNormal = mul(float4(iNormal,0.0),skinMatrix).xyz;
#endif

}
