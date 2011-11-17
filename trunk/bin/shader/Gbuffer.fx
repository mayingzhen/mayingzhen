float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

float shininess = 16.0h;

float4 depth_near_far_invfar;

float3 F32_3xI8_Compress(float f)
{
	float u,v,w;
	float res_u, res_v;

	u = floor(f * 256.0);
	res_u = f * 256.0 - u;
	v = floor(res_u * 256.0);
	res_v = res_u * 256.0 - v;
	w = floor(res_v * 256.0);

	return (1 / 256.0 * float3(u, v, w));
}

float F32_3xI8_Decompress(float3 vec)
{
	return (vec.x + vec.y * 1.0 / 256.0 + vec.z * 1.0 / 65536.0);
}


struct VS_OUT
{      
	float4 oPos : POSITION;
  	float4 oPos2 : TEXCOORD0;
  	float2 oTex : TEXCOORD1;
  	float3 oNormal : TEXCOORD2;	
};

VS_OUT GBufferVS( float4 pos : POSITION,
				  float3 normal : NORMAL,
         		  float2 texcoord : TEXCOORD0)
{
	VS_OUT vout;
   	vout.oPos = mul(pos, worldviewprojection);
   	vout.oPos2 = float4(0,0,vout.oPos.w * depth_near_far_invfar.z,0);
   	vout.oTex = texcoord;
   	vout.oNormal = mul(normal,(float3x3)worldview);
   	return vout;
}

struct PS_OUT
{
	half4 Pos : COLOR0;
  	half4 normal : COLOR1;
};
void GBufferPS( VS_OUT vout, out PS_OUT pout ) 
{
	pout.Pos = vout.oPos2.z;
  	pout.normal = float4( (normalize(vout.oNormal) + 1) * 0.5, shininess / 255.0f );
}

technique GBufferTech
{
	pass P0
	{
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
		ZEnable = true;
		ZWriteEnable = true;
	}
}
