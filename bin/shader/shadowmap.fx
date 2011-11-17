float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;


// HW shadow map
void VertShadow( float4 Pos : POSITION,
				 out float4 oPos : POSITION)
{
	oPos = mul( Pos, worldviewprojection );
}


technique RenderShadow
{
	pass p0
	{
		VertexShader = compile vs_1_1 VertShadow();
		
       	//ZEnable          = True;
        //AlphaBlendEnable = False;
        //Lighting         = False;
        //CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
	}		
}


//for PIX debug shader
/*
float4 depth_near_far_invfar;

void VertShadow( float4 Pos : POSITION,
				 out float4 oPos : POSITION,
				 out float4 oPos2 : TEXCOORD0)
{
	oPos = mul( Pos, worldviewprojection );
	oPos2 = mul( Pos, worldview );  
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
		
		//CullMode = none;
	}		
}
*/