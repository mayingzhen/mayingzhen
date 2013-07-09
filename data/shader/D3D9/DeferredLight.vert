float4x4 u_InvProjMatrix ;

	
struct VS_OUT
{
	float4 pos : POSITION;
	float2 oTc : TEXCOORD0;
	float3 oViewDir : TEXCOORD1;
};



void main( float4 pos : POSITION,
					  out VS_OUT vOut)
{
	vOut.pos = float4(pos.xyz ,1);
	
	vOut.oViewDir = mul(u_InvProjMatrix, float4(pos.xyz ,1)).xyz;
	
	vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
	vOut.oTc.y *= -1;
	vOut.oTc += 0.5f;
}








