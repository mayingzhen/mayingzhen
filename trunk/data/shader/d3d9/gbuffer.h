
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

PS_OUT GbufferPSout(float4 diffuse, float shininess, float4 normalDepth/*,out PS_OUT pout*/)
{
	PS_OUT pout;

	pout.diffuse = diffuse;

#ifdef ENCODENORMAL
	pout.normalShininess.xy = EncodeNormal( normalize(normalDepth.xyz) );
	pout.normalShininess.z = 0;
#else
	pout.normalShininess.xyz = normalDepth.xyz;
#endif 

	pout.normalShininess.w = shininess / 255.0f;

#ifndef HWDEPTH	
	pout.depth = normalDepth.w;
#endif

	return pout;
}