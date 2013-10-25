//float4 depth_near_far_invfar;

void main( in float2 oDepth : TEXCOORD0,
				  out float4 Color : COLOR )
{
	//Color = pos2.z * depth_near_far_invfar.z;
	Color = oDepth.x / oDepth.y;
}

