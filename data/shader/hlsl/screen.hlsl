

void vs_main(float2 iPos     : POSITION0,
			float2 iUV		: TEXCOORD0,
					out float2 oTex : TEXCOORD0,
					out float4 oPos : SV_POSITION)
{
	oPos = float4(iPos.x, iPos.y, 1.0, 1.0);
	oTex = iUV;
}



Texture2D tSrcColor : register(t0);
SamplerState sSrcColor : register( s0 );

void ps_main(float2 iUV      : TEXCOORD0,
        out float4 oColor   :SV_TARGET)
{
	oColor = tSrcColor.Sample(sSrcColor, iUV);
}
