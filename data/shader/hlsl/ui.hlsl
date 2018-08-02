cbuffer vertexBuffer : register(b0) 
{
    float4 uScaleTranslate;
};

struct VS_INPUT
{
    float2 pos : POSITION;
	float2 uv  : TEXCOORD0;
    float4 col : COLOR0;
};
            
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};
            
PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos.xy * uScaleTranslate.xy + uScaleTranslate.zw, 0.0, 1.0);
    output.col = input.col;
    output.uv  = input.uv;
    return output;
}

sampler sampler0;
Texture2D texture0;
float4 ps_main(PS_INPUT input) : SV_Target
{
    float4 out_col = input.col * texture0.Sample(sampler0, input.uv); 
    return out_col; 
}
