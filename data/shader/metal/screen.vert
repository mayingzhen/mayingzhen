
float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1;
	tex += 0.5;
	return tex;
}

void main(float2 iPos     : POSITION0,
			float2 iUV		: TEXCOORD0,
					out float2 oTex : TEXCOORD0,
					out float4 oPos : SV_POSITION)
{
	oPos = float4(iPos.x, iPos.y, 1.0, 1.0);
	oTex = TexCoordFromPos(oPos);
}
