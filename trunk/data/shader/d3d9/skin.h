// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------
#ifndef MAX_NUM_BONES
#define MAX_NUM_BONES 100
#endif

float4 boneDQ[MAX_NUM_BONES*2];


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

	float2x4 blendDQ = dq0*iBoneWeight.x;
	blendDQ += dq1*iBoneWeight.y;
	blendDQ += dq2*iBoneWeight.z;
	blendDQ += dq3*iBoneWeight.w;        

	blendDQ /= length(blendDQ[0]);

	return blendDQ;
}

float3 DQSkinPos(float3 iPos,float2x4 blendDQ)
{
	float3 finalPos = iPos.xyz + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iPos.xyz) + blendDQ[0].x*iPos.xyz);
	float3 trans = 2.0*(blendDQ[0].x*blendDQ[1].yzw - blendDQ[1].x*blendDQ[0].yzw + cross(blendDQ[0].yzw, blendDQ[1].yzw));
	finalPos += trans;

	return finalPos;
}


float3 SkinPos(float3 iPos,int4 iBoneIndex, float4 iBoneWeight)
{
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight);
	float3 finalPos = DQSkinPos(iPos,blendDQ);
	return finalPos;
}


void SkinPosNormal(float3 iPos,float3 iNormal,int4 iBoneIndex, float4 iBoneWeight, out float3 finalPos, out float3 finalNormal)
{
	float2x4 blendDQ = GetblendDQ(iBoneIndex,iBoneWeight);
	finalPos = DQSkinPos(iPos,blendDQ);

	finalNormal = iNormal + 2.0*cross(blendDQ[0].yzw, cross(blendDQ[0].yzw, iNormal) + blendDQ[0].x*iNormal);
}
