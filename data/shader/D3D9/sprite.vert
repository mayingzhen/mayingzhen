// Attributes
struct VS_IN
{
   float4 a_position : POSITION;
   float2 a_texCoord0 : TEXCOORD0;
   float4 a_color : COLOR0;
};

// Uniforms
uniform float4x4 u_projectionMatrix;

// Varyings
struct VS_OUT
{
   float4 v_position : POSITION;
   float2 v_texCoord : TEXCOORD0;
   float4 v_color : COLOR0;
};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    //Out.v_position = mul(In.a_position, u_projectionMatrix);
    Out.v_position = mul(u_projectionMatrix, In.a_position);
    Out.v_texCoord = In.a_texCoord0;
    Out.v_color = In.a_color;
    
    return Out;
}