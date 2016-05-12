Texture2D tSrcColor : register(t0);
SamplerState sSrcColor : register( s0 );

void main(float2 iUV      : TEXCOORD0,
        out float4 oColor   :SV_TARGET)
{
	oColor = tSrcColor.Sample(sSrcColor, iUV);
}