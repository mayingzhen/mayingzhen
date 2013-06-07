
// Uniforms
uniform sampler2D u_texture;

// Varyings
struct PS_IN
{
   float2   v_texCoord : TEXCOORD0;
   float4   v_color : COLOR0;
};


float4 main(PS_IN In) : COLOR0
{
    return In.v_color * tex2D(u_texture, In.v_texCoord);
}

