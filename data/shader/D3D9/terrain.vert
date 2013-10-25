// transformations
uniform float4x4 u_worldViewProjectionMatrix;


struct VS_INPUT
{
	float3	Pos		: POSITION;
	float2	UV1		: TEXCOORD0;
	float2	UV2		: TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 vTex0  : TEXCOORD0;
    float2 vTex1  : TEXCOORD1; 
    
#ifdef DeferredLight
   float2 v_defTc : TEXCOORD2;
#endif    
 
};

VS_OUTPUT main(const VS_INPUT v)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(u_worldViewProjectionMatrix, float4(v.Pos,1));
	Out.vTex0 = v.UV1;
	Out.vTex1 = v.UV2;
	
#ifdef DeferredLight
   Out.v_defTc = Out.Pos.xy / Out.Pos.w;
   Out.v_defTc.y *= -1;
   Out.v_defTc = Out.v_defTc * 0.5f + 0.5f;
#endif   
	
    return Out;
}




