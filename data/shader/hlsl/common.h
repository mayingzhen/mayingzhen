#ifndef D3D11

float4x4 g_matView;
float4x4 g_matViewInv;
float4x4 g_matProj;
float4x4 g_matProjInv;
float4x4 g_matViewProj;
float4x4 g_matWorldView;
float4x4 g_matWorldViewProj;
float4x4 g_matWorld;
float4 g_color;
float3 g_cSkyLight;
float3 g_cDirLight;
float3 g_vDirLight;
float3 g_vEyeWorldPos;
float2 g_vViewport;
float2 g_vViewportInv;
float4  g_vCameraNearFar;
float g_fTime;

float3 pos_extent;
float3 pos_center;
float4 tc_extent_center;

uniform float4 u_cSpecColor;
uniform float u_specPower;

#else

cbuffer FrameVS : register(b0)
{	
	float g_fTime;
}

cbuffer CameraVS : register(b1)
{
	float4x4 g_matView;
	float4x4 g_matViewInv;
	float4x4 g_matProj;
	float4x4 g_matProjInv;
	float4x4 g_matViewProj;

	float3 g_vEyeWorldPos;
	float2 g_vViewport;
	float2 g_vViewportInv;
	float4  g_vCameraNearFar;
}

cbuffer LightPS : register(b3)
{
	float3 g_cSkyLight;
	float3 g_cDirLight;
	float3 g_vDirLight;
}


#endif


//http://aras-p.info/texts/CompactNormalStorage.html
float2 EncodeNormal(float3 n)
{
	float2 enc = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
	enc = enc*0.5+0.5;
	return enc;

	//return( normal.x / (1.0f - normal.z), normal.y / (1.0f - normal.z) );
}

float3 DecodeNormal(float2 enc)
{	
// 	float fTemp = n.x * n.x + n.y * n.y;
// 	return (2 * n.x / (1 + fTemp), 2 * n.y / (1 + fTemp), (-1 + fTemp) / (1 + fTemp));

	float4 nn = float4(enc,0,0)*float4(2,2,0,0) + float4(-1,-1,1,-1);
	float l = dot(nn.xyz,-nn.xyw);
	nn.z = l;
	nn.xy *= sqrt(l);
	return nn.xyz * 2 + float3(0,0,-1);
}

// Encoding a [0, 1) float into a rgba8. From http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/
float4 EncodeFloatRGBA(float v)
{
	float4 enc = float4(1.0f, 255.0f, 65025.0f, 16581375.0f) * v;
	enc = frac(enc);
	enc -= enc.yzww * float4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 0);
	return enc;
}

float DecodeFloatRGBA(float4 rgba)
{
	return dot(rgba, float4(1, 1 / 255.0f, 1 / 65025.0f, 1 / 16581375.0f));
}


