
cbuffer ObjectVS : register(b5)
{
	float tex_coord_offset[8];
}

float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1;
	tex += 0.5;
	return tex;
}


void main(float3 pos : POSITION,
					out float4 oTex0 : TEXCOORD0,
					out float4 oTex1 : TEXCOORD1,
					out float4 oTex2 : TEXCOORD2,
					out float4 oTex3 : TEXCOORD3,
					out float2 oOriTex : TEXCOORD4,
					out float4 oPos : SV_POSITION)
{
	oPos = float4(pos.xyz ,1);

	float2 Tex0 = TexCoordFromPos(oPos);
	float4 tex[4];
	for (int i = 0; i < 4; ++ i)
	{
#if BLUR_X == 1
		tex[i] = Tex0.xyxy + float4(tex_coord_offset[i * 2 + 0], 0, tex_coord_offset[i * 2 + 1], 0);
#elif BLUR_Y == 1
		tex[i] = Tex0.xyxy + float4(0, tex_coord_offset[i * 2 + 0], 0, tex_coord_offset[i * 2 + 1]);
#endif 		
	}
	oTex0 = tex[0];
	oTex1 = tex[1];
	oTex2 = tex[2];
	oTex3 = tex[3];
	oOriTex = Tex0;
}

