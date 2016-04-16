
struct PS_OUT
{
	float4 diffuse : COLOR0;
	float4 normalShininess : COLOR1;
#ifndef HWDEPTH 	
	float4 depth : COLOR2;
#endif
};


void GBufferVSOut(float3 normal, float depth, out float4 normalDepth)
{
	normalDepth = 0;

	normalDepth.xyz = mul(float4(normal,0), g_matWorldView).xyz; 
#ifndef HWDEPTH   
	normalDepth.w = depth * g_vCameraNearFar.z;
#endif
}

PS_OUT GbufferPSout(float4 diffuse, float4 spec, float4 normalDepth)
{
	PS_OUT pout;
	
	const float3 RGB_TO_LUM = float3(0.299f, 0.587f, 0.114f);
	float specIntensity = dot(spec.xyz,RGB_TO_LUM);
	float specPower = spec.w;

	pout.diffuse = diffuse;

#ifdef ENCODENORMAL
	pout.normalShininess.xy = EncodeNormal( normalize(normalDepth.xyz) );
	pout.normalShininess.z = specIntensity;
#else
	pout.normalShininess.xyz = normalDepth.xyz;
#endif 

	pout.normalShininess.w = specPower / 255.0f;

#ifndef HWDEPTH	
	pout.depth = normalDepth.w;
#endif

	return pout;
}