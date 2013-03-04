//float4x4 worldviewprojection : worldviewprojection;
//float4x4 worldview : worldview;
float4x4 viewprojection : viewprojection;
//float4x4 view : view;

float shininess = 16.0h;

float4 depth_near_far_invfar;

static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

int CurNumBones = 2;

float3 F32_3xI8_Compress(float f)
{
	float u,v,w;
	float res_u, res_v;

	u = floor(f * 256.0);
	res_u = f * 256.0 - u;
	v = floor(res_u * 256.0);
	res_v = res_u * 256.0 - v;
	w = floor(res_v * 256.0);

	return (1 / 256.0 * float3(u, v, w));
}

float F32_3xI8_Decompress(float3 vec)
{
	return (vec.x + vec.y * 1.0 / 256.0 + vec.z * 1.0 / 65536.0);
}


struct VS_OUT
{      
	float4 oPos : POSITION;
  	float4 oPos2 : TEXCOORD0;
  	float2 oTex : TEXCOORD1;
  	float3 oNormal : TEXCOORD2;	
};

void SkinPos( float4 pos,
			  float3 normal,
			    float4 BlendWeights , 
				float4 BlendIndices,
				out float3 wPos,
				out float3 wNormal)
{
	wPos = 0;
	wNormal = 0.0f;  
	float LastWeight = 0.0f;
	// cast the vectors to arrays for use in the for loop below
	int4 Indices = D3DCOLORtoUBYTE4(BlendIndices);
    int   IndexArray[4]   = (int[4])Indices; 
    float WeightsArray[4] = (float[4])BlendWeights;
    for (int iBone = 0; iBone < CurNumBones-1; iBone++)
    {
        LastWeight = LastWeight + WeightsArray[iBone];   
        wPos += mul(pos, mWorldMatrixArray[IndexArray[iBone]]) * WeightsArray[iBone];
        wNormal += mul(normal, mWorldMatrixArray[IndexArray[iBone]]) * WeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 
    // Now that we have the calculated weight, add in the final influence
    wPos += (mul(pos, mWorldMatrixArray[IndexArray[CurNumBones-1]]) * LastWeight);
}

VS_OUT SkinGBufferVS( float4 pos : POSITION,
					  float4 BlendWeights :BLENDWEIGHT, 
				 	  float4 BlendIndices :BLENDINDICES,
				 	  float3 normal : NORMAL,
         		  	  float2 texcoord : TEXCOORD0 )
{
	VS_OUT vout = (VS_OUT)0;
	
	float3 wPos = 0;
	float3 wNormal = 0;  
	SkinPos(pos,normal,BlendWeights,BlendIndices,wPos,wNormal);
	
   	vout.oPos = mul( float4(wPos.xyz, 1.0f), viewprojection);
   	vout.oPos2 = float4(0,0,vout.oPos.w * depth_near_far_invfar.z,0);
   	vout.oTex = texcoord;
   	vout.oNormal = wNormal;//mul(wNormal,(float3x3)view);
   	return vout;
}

struct PS_OUT
{
	half4 Pos : COLOR0;
  	half4 normal : COLOR1;
};
void GBufferPS( VS_OUT vout, out PS_OUT pout ) 
{
	pout.Pos = vout.oPos2.z;
  	pout.normal = float4( (normalize(vout.oNormal) + 1) * 0.5, shininess / 255.0f );
}

technique SkinGBufferTech
{
	pass P0
	{
		VertexShader = compile vs_3_0 SkinGBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
		ZEnable = true;
		ZWriteEnable = true;
	}
}
