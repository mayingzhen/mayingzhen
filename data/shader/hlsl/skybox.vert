
cbuffer ObjectVS : register(b5)
{
	float4x4 inv_mvp;
};

void main(float2 iPos     : POSITION0,
		float2 iUV		: TEXCOORD0,
        
        // output
        out float4 oPos     : SV_POSITION,
        out float3 oUv      : TEXCOORD0)
{
    oPos = float4(iPos.x, iPos.y, 1, 1);
	oUv = mul(oPos, inv_mvp).xyz;	
}
