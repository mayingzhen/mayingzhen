float4 shininess = 16.0f;

struct VS_OUT
{      
	float4 oPos : POSITION;
  	float4 oPos2 : TEXCOORD0;
  	float2 oTex : TEXCOORD1;
  	float3 oNormal : TEXCOORD2;	
};


struct PS_OUT
{
	half4 Pos : COLOR0;
  	half4 normal : COLOR1;
};

void main( VS_OUT vout, out PS_OUT pout ) 
{
	pout.Pos = vout.oPos2.z;
  	pout.normal = float4( (normalize(vout.oNormal) + 1) * 0.5, shininess.x / 255.0f );
}