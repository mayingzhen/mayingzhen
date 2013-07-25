// transformations
uniform float4x4 u_worldViewProjectionMatrix;


struct VS_INPUT
{
	float4	Pos		: POSITION0;
	float2	UV1		: TEXCOORD0;
	float2	UV2		: TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 vTex0  : TEXCOORD0;
    float2 vTex1  : TEXCOORD1; 
    float2 oTc : TEXCOORD2;
 
};

VS_OUTPUT main(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(u_worldViewProjectionMatrix, v.Pos);
	Out.vTex0 = v.UV1;
	Out.vTex1 = v.UV2;
	
	Out.oTc = Out.Pos.xy / Out.Pos.w;
	Out.oTc.y *= -1;
	Out.oTc = Out.oTc * 0.5f + 0.5f;
	
    return Out;
}




