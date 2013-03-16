float4x4 viewprojection : viewprojection;

static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

int CurNumBones = 2;

void SkinPos( float4 pos,
			    float4 BlendWeights , 
				float4 BlendIndices,
				out float3 wPos)
{
	wPos = 0;
	float LastWeight = 0.0f;
	// cast the vectors to arrays for use in the for loop below
	int4 Indices = D3DCOLORtoUBYTE4(BlendIndices);
    int   IndexArray[4]   = (int[4])Indices; 
    float WeightsArray[4] = (float[4])BlendWeights;
    for (int iBone = 0; iBone < CurNumBones-1; iBone++)
    {
        LastWeight = LastWeight + WeightsArray[iBone];   
        wPos += mul(pos, mWorldMatrixArray[IndexArray[iBone]]) * WeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 
    // Now that we have the calculated weight, add in the final influence
    wPos += (mul(pos, mWorldMatrixArray[IndexArray[CurNumBones-1]]) * LastWeight);
}


// HW shadow map
void VertShadow( float4 Pos : POSITION,
				 float4 BlendWeights :BLENDWEIGHT, 
				 float4 BlendIndices :BLENDINDICES,	
				 out float4 oPos : POSITION )
{
	float3 wPos = 0;
	SkinPos(Pos,BlendWeights,BlendIndices,wPos);

	oPos = mul(  float4(wPos.xyz, 1.0f), viewprojection );
}


technique RenderShadow
{
	pass p0
	{
		VertexShader = compile vs_1_1 VertShadow();
		
        //ZEnable          = True;
        //AlphaBlendEnable = False;
       // Lighting         = False;
        //CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
	}		
}



/*
float4 depth_near_far_invfar;

void VertShadow( float4 Pos : POSITION,
				 float4 BlendWeights :BLENDWEIGHT, 
				 float4 BlendIndices :BLENDINDICES,		
				 out float4 oPos : POSITION,
				 out float4 oPos2 : TEXCOORD0)
{
	float3 wPos = 0;
	SkinPos(Pos,BlendWeights,BlendIndices,wPos);
	oPos = mul(  float4(wPos.xyz, 1.0f), viewprojection );
	oPos2 = float4(wPos,1);  
}

void PixelShadow( float4 pos2 : TEXCOORD0,
				  out float4 Color : COLOR )
{
	Color = pos2.z * depth_near_far_invfar.z;
}


technique RenderShadow
{
	pass p0
	{
		VertexShader = compile vs_1_1 VertShadow();
		PixelShader = compile ps_2_0 PixelShadow();
	}		
}
*/