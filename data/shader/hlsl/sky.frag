Texture2D tOctave0 : register(t0);
SamplerState sOctave0 : register(s0);

Texture2D tOctave1 : register(t1);
SamplerState sOctave1 : register(s1);

Texture2D tOctave2 : register(t2);
SamplerState sOctave2 : register(s2);

Texture2D tOctave3 : register(t3);
SamplerState sOctave3 : register(s3);

cbuffer ObjectPS : register(b5)
{
	float4 u_skycolorAbove;
	float4 u_skycolorBelow;
	float4 u_cloudcolor;
	float u_speed;
	float u_emptiness;
	float u_sharpness;
}

void main(float4 iUV0      : TEXCOORD0,
			float4 iUV1      : TEXCOORD1,
        out float4 oColor   :SV_TARGET)
{
	
	float4 n0 = tOctave0.Sample(sOctave0, iUV0.xy);
	float4 n1 = tOctave1.Sample(sOctave1, iUV0.zw);
	float4 n2 = tOctave2.Sample(sOctave2, iUV1.xy);
	float4 n3 = tOctave3.Sample(sOctave3, iUV1.zw);

	float4 fbm = 0.5 * n0 + 0.25 * n1 + 0.125 * n2 + 0.0625 * n3;
	fbm = (clamp(fbm, u_Emptiness, u_Sharpness) -  u_Emptiness) / (u_Sharpness - u_Emptiness);

	float4 ray = float4(0.0, 0.2, 0.4, 0.6);
	float amount = dot(max(fbm - ray, 0), float4(0.25, 0.25, 0.25, 0.25));

	oColor.rgb = amount * u_cloudcolor.rgb +  2.0 * (1.0 - amount) * 0.4;
	oColor.a = amount * 1.5;

	return oColor;
}