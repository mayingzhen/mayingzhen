
// Attributes
struct VS_IN
{
   float2 a_position : POSITION;
   
   float4 a_gradientPos: TEXCOORD0;
};


// Varyings
struct VS_OUT
{
	float4 v_position : SV_POSITION;

	float4 Color : TEXCOORD0;
	float4 GradientPos : TEXCOORD1;
};



VS_OUT vs_main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

  	Out.Color = vec4(0.035);
  	Out.GradientPos = inGradientPos.x;
  	Out.v_position = vec4(inPos.xy, 1.0, 1.0);

    return Out;
}


void ps_main(VS_OUT In,
out float4 outColor : SV_TARGET 
) 
{
	float3 color = texture(samplerGradientRamp, vec2(inGradientPos, 0.0)).rgb;
	outColor.rgb = texture(samplerColorMap, gl_PointCoord).rgb * color;
	outColor.a = 1.0;
}


