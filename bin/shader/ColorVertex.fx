float4x4 gmVP : VIEWPROJECTION;

struct VertexOutput
{
	float4 Ph	: POSITION;
	float4 I	: COLOR0;
};

VertexOutput mainVS(float3 InPw : POSITION
					,float4 InKd : COLOR0
					,uniform float4x4 mVP)
{
	VertexOutput op = (VertexOutput)0;
	op.Ph = mul(float4(InPw,1.0),mVP);
	op.I  = InKd;
	return op;
}

void mainPS(VertexOutput ip
			,out	float4 oI : COLOR0)
{
	oI = ip.I;
}

technique LineShader
{
	pass p0
	{

		VertexShader = compile vs_2_0 mainVS(gmVP);
		PixelShader = compile ps_2_0 mainPS();

		Zenable = true;
		AlphaBlendEnable = false;
		Cullmode = none;
	}
}
