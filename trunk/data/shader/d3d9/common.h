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
float3 g_cSpecularLight;
float3 g_vEyeWorldPos;
float3 g_vAtWorldPos;
float2 g_vViewport;
float2 g_vViewportInv;
float4  g_vCameraNearFar;
float g_fTime;

//http://aras-p.info/texts/CompactNormalStorage.html
float2 EncodeNormal(float3 normal)
{
	return normalize(normal.xy) * sqrt(normal.z * 0.5 + 0.5);
	//return( normal.x / (1.0f - normal.z), normal.y / (1.0f - normal.z) );
}

float3 DecodeNormal(float2 n)
{
// 	float fTemp = n.x * n.x + n.y * n.y;
// 	return (2 * n.x / (1 + fTemp), 2 * n.y / (1 + fTemp), (-1 + fTemp) / (1 + fTemp));
	float3 normal;
	normal.z = dot(n, n) * 2 - 1;
	normal.xy = normalize(n) * sqrt(1 - normal.z * normal.z);
	return normal;
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


