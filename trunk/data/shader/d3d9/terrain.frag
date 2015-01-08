

#ifdef DeferredLight   
uniform sampler2D u_textureLightDiffuse;
uniform sampler2D u_textureLightSpecular;
#endif


sampler tBlendingMap;

sampler tDetailMap0;
sampler tDetailMap1;

sampler tBumpMap0;
sampler tBumpMap1;

uniform float2 uBlendingOffset;


struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float2 UV		: TEXCOORD0;
    float4 DetailUV	: TEXCOORD1;
	float4 Color	: TEXCOORD2;
	float4 WorldPos : TEXCOORD3;
#if USING_SHADOW != 0
	float2 RandDirTC : TEXCOORD4;
	float4 ShadowPos[MAX_FRUSTUM_SPLIT_NUM] : TEXCOORD5;
#endif	

};

float4 main(VS_OUTPUT In) : COLOR
{
	float4 cBlend = tex2D(tBlendingMap, In.UV + uBlendingOffset);

    // œ∏Ω⁄Õº
#if LAYER==1
 	float4 cDetailMap0 = tex2D(tDetailMap0, In.DetailUV.xy);
	float4 oColor = cDetailMap0;
#elif LAYER==2
    float4 cDetailMap0 = tex2D(tDetailMap0, In.DetailUV.xy);
    float4 cDetailMap1 = tex2D(tDetailMap1, In.DetailUV.zw);
    float4 oColor = cDetailMap0 * cBlend.a + cDetailMap1 * (1.0 - cBlend.a);
#endif


// “ı”∞
#if USING_SHADOW != 0
	oColor.rgb *= DoShadowMapping(In.ShadowPos,In.RandDirTC,In.WorldPos.w);
#endif

	oColor.a = In.Color.a;		
	
	return oColor;
}


