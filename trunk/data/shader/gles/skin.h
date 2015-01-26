

#define BONE_DQ

#ifdef BONE_DQ
#ifndef MAX_DQ_NUM_BONES
#define MAX_DQ_NUM_BONES 100
#endif
uniform vec4 boneDQ[MAX_DQ_NUM_BONES*2];
uniform vec4 boneScale[MAX_DQ_NUM_BONES/4];
#else
#ifndef MAX_MAT_NUM_BONES
#define MAX_MAT_NUM_BONES 75
#endif
uniform vec4 boneMatrix[MAX_MAT_NUM_BONES * 3];
#endif


#ifdef BONE_DQ
float GetBoneScale(vec4 iBoneIndex, vec4 iBoneWeight)
{
#if BONE==1
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][int(mod(iBoneIndex.x,4.0))]*iBoneWeight.x;
#elif BONE==2
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][int(mod(iBoneIndex.x,4.0))]*iBoneWeight.x;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][int(mod(iBoneIndex.y,4.0))]*iBoneWeight.y;
#elif BONE==3
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][int(mod(iBoneIndex.x,4.0))]*iBoneWeight.x;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][int(mod(iBoneIndex.y,4.0))]*iBoneWeight.y;
	fBoneScale += boneScale[int(iBoneIndex.z*0.25)][int(mod(iBoneIndex.z,4.0))]*iBoneWeight.z;
#else
	float fBoneScale = boneScale[int(iBoneIndex.x*0.25)][int(mod(iBoneIndex.x,4.0))]*iBoneWeight.x;
	fBoneScale += boneScale[int(iBoneIndex.y*0.25)][int(mod(iBoneIndex.y,4.0))]*iBoneWeight.y;
	fBoneScale += boneScale[int(iBoneIndex.z*0.25)][int(mod(iBoneIndex.z,4.0))]*iBoneWeight.z;
	fBoneScale += boneScale[int(iBoneIndex.w*0.25)][int(mod(iBoneIndex.w,4.0))]*iBoneWeight.w;
#endif
	return fBoneScale;
}

void GetblendDQ(vec4 iBoneIndex, vec4 iBoneWeight,out vec4 blendDQ[2])
{

	int y0 = int(iBoneIndex.x)*2;
	int y1 = int(iBoneIndex.y)*2;
	int y2 = int(iBoneIndex.z)*2;
	int y3 = int(iBoneIndex.w)*2;
	float dq1 = sign(dot(boneDQ[y0], boneDQ[y1]));
	float dq2 = sign(dot(boneDQ[y0], boneDQ[y2]));
	float dq3 = sign(dot(boneDQ[y0], boneDQ[y3]));
	//vec4 blendDQ[2];
#if BONE==1
	blendDQ[0]  = vec4(iBoneWeight.x*boneDQ[y0]);
	blendDQ[1]  = vec4(iBoneWeight.x*boneDQ[y0+1]);
#elif BONE==2
	blendDQ[0]  = vec4(iBoneWeight.x*boneDQ[y0]);
	blendDQ[0] += vec4(iBoneWeight.y*boneDQ[y1])*dq1;
	blendDQ[1]  = vec4(iBoneWeight.x*boneDQ[y0+1]);
	blendDQ[1] += vec4(iBoneWeight.y*boneDQ[y1+1])*dq1;
#elif BONE==3
	blendDQ[0]  = vec4(iBoneWeight.x*boneDQ[y0]);
	blendDQ[0] += vec4(iBoneWeight.y*boneDQ[y1])*dq1;
	blendDQ[0] += vec4(iBoneWeight.z*boneDQ[y2])*dq2;
	blendDQ[1]  = vec4(iBoneWeight.x*boneDQ[y0+1]);
	blendDQ[1] += vec4(iBoneWeight.y*boneDQ[y1+1])*dq1;
	blendDQ[1] += vec4(iBoneWeight.z*boneDQ[y2+1])*dq2;
#else
	blendDQ[0]  = vec4(iBoneWeight.x*boneDQ[y0]);
	blendDQ[0] += vec4(iBoneWeight.y*boneDQ[y1])*dq1;
	blendDQ[0] += vec4(iBoneWeight.z*boneDQ[y2])*dq2;
	blendDQ[0] += vec4(iBoneWeight.w*boneDQ[y3])*dq3;
	blendDQ[1]  = vec4(iBoneWeight.x*boneDQ[y0+1]);
	blendDQ[1] += vec4(iBoneWeight.y*boneDQ[y1+1])*dq1;
	blendDQ[1] += vec4(iBoneWeight.z*boneDQ[y2+1])*dq2;
	blendDQ[1] += vec4(iBoneWeight.w*boneDQ[y3+1])*dq3;
#endif

	float len = length(blendDQ[0]);
	blendDQ[0] = blendDQ[0]/len;
	blendDQ[1] = blendDQ[1]/len;
}

vec3 DQSkinPos(vec3 iPos,float fBoneScale,vec4 blendDQ[2])
{
#ifdef BONESCALE
	iPos.xyz *= fBoneScale;
#endif

	vec3 finalPos = iPos.xyz + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iPos.xyz) + blendDQ[0].x*iPos.xyz);
	vec3 trans = 2.0*(blendDQ[0].x*blendDQ[1].yzw - blendDQ[1].x*blendDQ[0].yzw + cross(blendDQ[0].yzw, blendDQ[1].yzw));
	finalPos += trans;

	return finalPos;
}

#else


mat4 GetSkinMatrix(vec4 iBoneIndex, vec4 iBoneWeight)
{
	ivec4 idx = ivec4(iBoneIndex) * 3;
	const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);

#if BONE==1
	return mat4(boneMatrix[idx.x], boneMatrix[idx.x + 1], boneMatrix[idx.x + 2], lastColumn) * iBoneWeight.x;
#elif BONE==2
	return mat4(boneMatrix[idx.x], boneMatrix[idx.x + 1], boneMatrix[idx.x + 2], lastColumn) * iBoneWeight.x +
		mat4(boneMatrix[idx.y], boneMatrix[idx.y + 1], boneMatrix[idx.y + 2], lastColumn) * iBoneWeight.y ;
#elif BONE==3
	return mat4(boneMatrix[idx.x], boneMatrix[idx.x + 1], boneMatrix[idx.x + 2], lastColumn) * iBoneWeight.x +
		mat4(boneMatrix[idx.y], boneMatrix[idx.y + 1], boneMatrix[idx.y + 2], lastColumn) * iBoneWeight.y +
		mat4(boneMatrix[idx.z], boneMatrix[idx.z + 1], boneMatrix[idx.z + 2], lastColumn) * iBoneWeight.z;
#else
	return mat4(boneMatrix[idx.x], boneMatrix[idx.x + 1], boneMatrix[idx.x + 2], lastColumn) * iBoneWeight.x +
		mat4(boneMatrix[idx.y], boneMatrix[idx.y + 1], boneMatrix[idx.y + 2], lastColumn) * iBoneWeight.y +
		mat4(boneMatrix[idx.z], boneMatrix[idx.z + 1], boneMatrix[idx.z + 2], lastColumn) * iBoneWeight.z +
		mat4(boneMatrix[idx.w], boneMatrix[idx.w + 1], boneMatrix[idx.w + 2], lastColumn) * iBoneWeight.w;
#endif
}

#endif

vec3 SkinPos(vec3 iPos,vec4 iBoneIndex, vec4 iBoneWeight)
{
#ifdef BONE_DQ
	vec4 blendDQ[2];
	GetblendDQ(iBoneIndex,iBoneWeight,blendDQ);

	float fBoneScale = GetBoneScale(iBoneIndex,iBoneWeight);
	
	vec3 finalPos = DQSkinPos(iPos,fBoneScale,blendDQ);

#else
	vec3 finalPos =  (vec4(iPos,1.0) * GetSkinMatrix(iBoneIndex,iBoneWeight)).xyz;

#endif

	return finalPos;
}

void SkinPosNormal(vec3 iPos,vec3 iNormal,vec4 iBoneIndex, vec4 iBoneWeight, out vec3 finalPos, out vec3 finalNormal)
{
#ifdef BONE_DQ
	vec4 blendDQ[2];
	GetblendDQ(iBoneIndex,iBoneWeight,blendDQ);

	float fBoneScale = GetBoneScale(iBoneIndex,iBoneWeight);
	
	finalPos = DQSkinPos(iPos,fBoneScale,blendDQ);

	finalNormal = iNormal + 2.0 * cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iNormal) + blendDQ[0].x*iNormal);

#else
	finalPos = (vec4(iPos,1.0) * GetSkinMatrix(iBoneIndex,iBoneWeight)).xyz;

	finalNormal = (vec4(iNormal,0.0) * GetSkinMatrix(iBoneIndex,iBoneWeight)).xyz;
#endif
}