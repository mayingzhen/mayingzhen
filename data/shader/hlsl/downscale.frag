/////////////////////////////////////////////////////////////////////////
//
//
//

struct PS_IN
{
	float2 uv0;
};


uniform float4 gUVOffsets[SCALE];
uniform SamplerState gTex_Color;

float4 ps_main(PS_IN In) : COLOR
{
	float4 clr = 0;
	
	for (int i = 0; i < SCALE; ++i)
	{
		clr += tex2D(gTex_Color, In.uv0 + gUVOffsets[i].xy);
	}
	
	clr /= SCALE;

	return clr;
}
