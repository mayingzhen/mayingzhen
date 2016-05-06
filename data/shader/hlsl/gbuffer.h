
struct PS_OUT
{
	float4 diffuse : SV_TARGET0;
	float4 normalShininess : SV_TARGET1;
};

PS_OUT GbufferPSout(float4 diffuse, float4 spec, float3 viewNormal)
{
	PS_OUT pout;
	
	const float3 RGB_TO_LUM = float3(0.299f, 0.587f, 0.114f);
	float specIntensity = dot(spec.xyz,RGB_TO_LUM);
	float specPower = spec.w;

	pout.diffuse = diffuse;

#ifdef ENCODENORMAL
	pout.normalShininess.xy = EncodeNormal( normalize(viewNormal.xyz) );
	pout.normalShininess.z = specIntensity;
#else
	pout.normalShininess.xyz = viewNormal.xyz;
#endif 

	pout.normalShininess.w = specPower / 255.0f;

	return pout;
}