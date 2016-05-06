#include"common.h"
#include"depth.h"


Texture2D tNormalMap;
SamplerState sNormalMap;

Texture2D tRandomMap;
SamplerState sRandomMap;


float2 TexCoordFromPos(float4 pos)
{
	float2 tex = pos.xy / 2;
	tex.y *= -1.0/*KLAYGE_FLIPPING*/; // openGL KLAYGE_FLIPPING=1.0
	tex += 0.5;
	return tex;
}


float CalcAlchemyObscurance(float2 tc, float3 pos_es, float3 x_dir, float3 y_dir, float3 normal, float radius, float depth_center)
{
	float rotate = tRandomMap.Sample(sRandomMap,tc * 1.5f).x;
	float sz, cz;
	sincos(rotate, sz, cz);

	x_dir *= radius;
	y_dir *= radius;

	float2 sphere_xy[2] = {0.66667, 0,
					0, 0.33333};

	float sqr_radius = radius * radius;
	float obscurance = 0;
	float total_weight = 0;
	for (int i = 0; i < 2; ++ i)
	{
		float2 sxy = sphere_xy[i];
		sxy = float2(dot(sxy, float2(cz, -sz)), dot(sxy, float2(sz, cz)));
		for (int j = 0; j < 2; ++ j)
		{
			float3 sample_point_offset = sxy.x * x_dir + sxy.y * y_dir;
			float3 sample_point = pos_es + sample_point_offset;
			float4 sample_point_ss = mul(float4(sample_point, 1), g_matProj);

			float2 tc_sample = TexCoordFromPos(sample_point_ss / sample_point_ss.w);
			float depth_sample = GetLinearDepth(tc_sample);//ReadAFloat(depth_tex.Sample(point_sampler, tc_sample), far_plane.x);
			float3 pos_es_sample = sample_point * (depth_sample / -sample_point.z);
			float3 dir_es_sample = pos_es_sample - pos_es;

			float epsilon = 0.0001;
			float beta = 0.02;

			float dist_sqr = dot(dir_es_sample, dir_es_sample);
			if (dist_sqr < sqr_radius)
			{
				obscurance += max(0.0f, dot(dir_es_sample, normal) - depth_center * beta) / (dist_sqr + epsilon);
				total_weight += 1.0f;
			}

			sxy = -sxy;
		}
	}

	float delta = 1.0f;
	return (total_weight > 1e-6f) ? 2.0f * delta * obscurance / total_weight : 0;
}


void main(float4 WPos	    : VPOS,
		float2 iUV      : TEXCOORD0,
		float3 view_dir : TEXCOORD1,
        out float4 oColor   :SV_TARGET)
{
	float radius = 1.5;
	float obscurance = 1.0;

	float depth = GetLinearDepth(iUV);
	float4 normalSample = tNormalMap.Sample(sNormalMap,iUV);
	float3 normal = DecodeNormal(normalSample.xy);
	normal = mul(normal, (float3x3)g_matView);

	view_dir = normalize(view_dir);
	float3 pos_es = view_dir * (depth / -view_dir.z);

	float3 y_dir = normalize(cross(normal, float3(0, 0, 1)));
	float3 x_dir = cross(y_dir, normal);

	obscurance = CalcAlchemyObscurance(iUV, pos_es, x_dir, y_dir, normal, radius, depth);
	float occ = max(0.0f, 1.0f - obscurance);
	oColor = float4(occ, occ, occ, 1);
}

