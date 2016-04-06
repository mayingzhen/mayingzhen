#include"common.h"
#include"smaa.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------
texture mtDepthTexture;
sampler tSrcColor
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_LINEAR";
	string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
>
 = sampler_state
{
    Texture = (mtDepthTexture);
};


void main(float2 texcoord      : TEXCOORD0,
		float4 offset[3]	 : TEXCOORD1,
        
        out float4 oColor   :COLOR0)
{
	oColor = SMAADepthEdgeDetectionPS(texcoord,offset,tSrcColor);
}

technique t
{
	pass p0
	<
        string vertexElement0 = "0,0,VET_FLOAT2,VES_POSITION,0";
		string vertexElement1 = "0,8,VET_FLOAT2,VES_TEXTURE_COORDINATES,0";
		
		string CullingMode = "CULL_NONE";
		string SceneBlending = "SBF_ONE,SBF_ZERO,SBO_ADD";
		string DepthCheckEnabled = "false";
		string DepthWriteEnabled = "false";
		string DepthFunction = "CMPF_ALWAYS_PASS";
	>
	{
    #ifdef D3D9A
        VertexShader = compile vs_2_0 vs();
        PixelShader = compile ps_2_0 ps();
    #else
        VertexShader = compile vs_3_0 vs();
        PixelShader = compile ps_3_0 ps();
    #endif
	}
}