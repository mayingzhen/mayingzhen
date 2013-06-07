float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;

#define MAX_SKIN_MATRIX 60
#define MAX_BLEND_BONE 4
float4x3    mSkinMatrixArray[MAX_SKIN_MATRIX] : WORLDMATRIXARRAY;

int CurNumBones = 4;


// HW shadow map
void HWVertShadow( float3 Pos : POSITION,
				 out float4 oPos : POSITION)
{
	oPos = mul( float4(Pos.xyz,1.0f), worldviewprojection );
}


void SkinPos( float3 pos ,
			    float4 BlendWeights , 
					int4 BlendIndices,
					out float3 sPos)
{
	sPos = 0;
  int   IndexArray[4]   = (int[4])BlendIndices; 
  float WeightArray[4] = (float[4])BlendWeights;
	for (int iBone = 0; iBone < MAX_BLEND_BONE; ++iBone)
	{
		sPos += mul(float4(sPos.xyz, 1.0f), mSkinMatrixArray[IndexArray[iBone]]).xyz * WeightArray[iBone];
	}
}

void SkinHWVertShadow( float4 Pos : POSITION,
				 float4 BlendWeights :BLENDWEIGHT, 
				 float4 BlendIndices :BLENDINDICES,	
				 out float4 oPos : POSITION )
{
	float3 sPos = 0;
	SkinPos(Pos,BlendWeights,BlendIndices,sPos);

	HWVertShadow(sPos,oPos);
	//oPos = mul(  float4(wPos.xyz, 1.0f), viewprojection );
}



technique HWRenderShadow
{
	pass p0
	{
		VertexShader = compile vs_1_1 HWVertShadow();
		
       	//ZEnable          = True;
        //AlphaBlendEnable = False;
        //Lighting         = False;
        //CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
	}		
}


technique SkinHWRenderShadow
{
	pass p0
	{
		VertexShader = compile vs_1_1 SkinHWVertShadow();
		
        //ZEnable          = True;
        //AlphaBlendEnable = False;
       // Lighting         = False;
        //CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
	}		
}






