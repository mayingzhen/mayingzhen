// Uniforms
uniform float4x4 u_worldViewProjectionMatrix;

uniform float2 uCellAmount;
uniform float2 uDetailScale;
uniform float4 uDetailOffSet;
uniform float4 uDetailRotate;
uniform float uCurMaterialID;


struct VS_INPUT
{
	float3	Pos		: POSITION;
	float2  UV      : TEXCOORD0;
    float4  Normal  : TEXCOORD1;
    float3  Tan     : TEXCOORD2;
	float4  Color	: COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;
	float4 Color	: TEXCOORD2;
	float4 WorldPos : TEXCOORD3;
#if USING_SHADOW != 0
	float2 RandDirTC : TEXCOORD4;
	float4 ShadowPos[MAX_FRUSTUM_SPLIT_NUM] : TEXCOORD5;
#endif	

};


float2 GetDetaiUV(float2 oUVPixel,float fDetailScale, float2 fDetailRot, float2 uDetailOffSet)
{
	float2 uv = oUVPixel * fDetailScale;

#ifdef DETAILROTATE
	float rsin = fDetailRot.x;
    float rcos = fDetailRot.y;
    float2x2 rotateP = { rcos, -rsin, rsin, rcos };
  
	uv = mul(uv, rotateP);
#endif

	uv += uDetailOffSet;

	return uv;
}

VS_OUTPUT main(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.WorldPos = float4(v.Pos, 1.0);
	Out.Pos = mul(Out.WorldPos,u_worldViewProjectionMatrix);
	Out.WorldPos.w = Out.Pos.w;

    Out.UV = v.UV;
    
#if USING_SHADOW != 0
	GetShadowPos(Out.WorldPos.xyz,Out.WorldPos.w,Out.ShadowPos,Out.RandDirTC);
#endif

#if LAYER==1
	Out.DetailUV.xy = GetDetaiUV( v.UV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
#elif LAYER==2
    Out.DetailUV.xy = GetDetaiUV( v.UV * uCellAmount, uDetailScale.x, uDetailRotate.xy, uDetailOffSet.xy ); 
    Out.DetailUV.zw = GetDetaiUV( v.UV * uCellAmount, uDetailScale.y, uDetailRotate.zw, uDetailOffSet.zw );
#endif

	Out.Color = v.Color;
	float fWeight = saturate(1 - abs(v.Color.a * 255.0 - uCurMaterialID));	
	Out.Color.a = fWeight;

    return Out;
}




