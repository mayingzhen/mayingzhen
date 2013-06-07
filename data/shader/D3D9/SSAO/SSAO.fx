float4x4 worldviewprojection : worldviewprojection;
float4x4 worldview : worldview;
float4x4 proj : proj;

float4 depth_near_far_invfar;

float vo_intensity = 2.14;
float2 sphereXy[2] = { float2(0.66667,0), float2(0,0.33333) };


texture g_TextureSrcPos;  // Gbuffer
sampler g_SamplerSrcPos = sampler_state
{
	Texture = <g_TextureSrcPos>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


texture g_TextureSrcNormal; // Gbuffer
sampler g_SamplerSrcNormal = sampler_state
{
	Texture = <g_TextureSrcNormal>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


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

float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1;
	tex += 0.5;
	return tex;
}

void SSAOVS( float4 pos : POSITION,
			 out float2 oTex : TEXCOORD0,
			 out float3 oViewDir : TEXCOORD1,
			 out float4 oPos : POSITION,
			 out float oW : TEXCOORD2 )
{
	//// wordviewproj = 1£¬wordview = inv_proj
	
	//oTex = TexCoordFromPos(pos);
	//oPos = pos;
	//oViewDir = mul( float4(pos.xyz ,1), worldview ).xyz;	
	
	oPos = mul( float4(pos.xyz ,1), worldviewprojection );
	
	oViewDir = mul( float4(pos.xyz ,1), worldview ).xyz;
	
	oTex = oPos.xy / oPos.w * 0.5f;
	oTex.y *= -1;
	oTex += 0.5f;
	
	oW = oPos.w;
	oTex *= oW;
}

float CalcObscurance(float2 tc, float3 viewPos, float3 xDir, float3 yDir, float3 normal, float radius)
{
	float sqrRadius = radius * radius;
	float obscurance = 0;
	float totalWeight = 0;
	
	for ( int i = 0; i < 2; ++i )
	{
		float2 sxy = sphereXy[i];
		for ( int j = 0; j < 2; ++j )
		{
			float3 samplePointOffset = ( sxy.x * xDir + sxy.y * yDir ) * radius;
			float3 samplePoint = viewPos + samplePointOffset;
			float4 samplePointSS = mul( float4(samplePoint,1), proj );
			
			float2 sampleTc = TexCoordFromPos( samplePointSS / samplePointSS.w ); 
			float sampleDepth = F32_3xI8_Decompress( tex2D(g_SamplerSrcPos, sampleTc).xyz );
			sampleDepth *= depth_near_far_invfar.y;
			
			float3 sampleViewDir = normalize(samplePoint);
			float v = dot(viewPos, sampleViewDir);
			float disc = sqrRadius - (dot(viewPos, viewPos) - v * v);
			float intersectT = v - sqrt(disc);
			
			float zs;
			if (sampleDepth <= intersectT * sampleViewDir.z)
			{
				float3 sampleViewPos = sampleViewDir * intersectT;
				zs = dot(sampleViewPos - viewPos, normal);
				obscurance += zs;
			}
			else
			{
				float3 sampleViewPos = sampleViewDir * (sampleDepth / sampleViewDir.z);	
				float3 sphereSamplePos = mul(float3x3(xDir,yDir,normal), sampleViewPos - viewPos);
				zs = sqrt( sqrRadius - dot(sphereSamplePos.xy, sphereSamplePos.xy) );
				obscurance += clamp(sphereSamplePos.z, 0, zs);
			}
			totalWeight += zs;
			
			sxy = -sxy;
		}
	}
	
	return ( totalWeight > 0.000001f ) ? obscurance / totalWeight : 0;
}


float4 SSAOPS( float2 tc : TEXCOORD0,
               float3 viewDir : TEXCOORD1,
               float W : TEXCOORD2 ) : COLOR
{
	tc /= W;

	float radius = 0.5f;
	//float radius = 50.0f;
	float obscurance = 1;
	
	viewDir = normalize(viewDir);
	
	float depth = F32_3xI8_Decompress( tex2D(g_SamplerSrcPos, tc).xyz );
	depth *= depth_near_far_invfar.y;
	float3 viewPos = viewDir * ( depth / viewDir.z );	
	
	float4 srcNormal = tex2D( g_SamplerSrcNormal, tc);
	float3 viewNormal = srcNormal.xyz * 2 - 1;
	
	float3 yDir = cross( viewNormal, float3(0,0,1) );
	float3 xDir = cross( yDir, viewNormal );
	
	obscurance = CalcObscurance(tc, viewPos, xDir, yDir, viewNormal, radius );
	return float4( 1 - saturate(vo_intensity * obscurance), depth, 0, 0 );
	//return float4( 1 - saturate(vo_intensity * obscurance), 0, 0, 0 );	
}

technique SSAO
{
	pass SSAO
	{
		VertexShader = compile vs_3_0 SSAOVS();
		PixelShader =  compile ps_3_0 SSAOPS();
		
		CullMode = cw;
	}	
}