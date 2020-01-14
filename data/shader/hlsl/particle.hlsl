#include "common.h"

struct PS_IN
{
	float4 pos : SV_Position;
	float4 clr : COLOR0;
	float4 PosSS : TEXCOORD0;
	float4 CenterView : TEXCOORD1;
	float3 view_dir : TEXCOORD2;
	float2 tex : TEXCOORD3;
};

cbuffer ObjectVS : register(b5)
{
	float point_radius;
	float4 init_pos_life;
	float4x4 ps_model_mat;
	
	float accumulate_time;
	float elapse_time;
}

void vs_main(float4 pos : POSITION,
					out float4 CenterView_Life : SV_Position)
{
	CenterView_Life = float4(mul(float4(pos.xyz, 1), g_matView).xyz, pos.w);
}


[maxvertexcount(4)]
void gs_main(point float4 input[1] : SV_Position, inout TriangleStream<PS_IN> out_stream)
{
	float4 CenterView_Life = input[0];

	if (CenterView_Life.w > 0)
	{
		PS_IN gs_out;

		for (int i = 0; i < 4; ++ i)
		{
			float2 tex = float2((i & 1) ? 1 : -1, (i & 2) ? -1 : 1);

			float4 view_pos = float4(CenterView_Life.xyz, 1);
			gs_out.CenterView = view_pos;
			view_pos.xy += tex * point_radius;
			gs_out.pos = mul(view_pos, g_matProj);
			gs_out.clr = float4(1, 1, 1, CenterView_Life.w);
			gs_out.PosSS = gs_out.pos;
			gs_out.view_dir = view_pos.xyz;
			gs_out.tex = tex * 0.5f + 0.5f;

			out_stream.Append(gs_out);
		}
	}
}

Texture2D particle_tex : register(t0);
SamplerState bilinear_sampler : register(s0);

float4 ps_main(PS_IN ps_in) : SV_Target
{
	ps_in.CenterView /= ps_in.CenterView.w;
	float3 intersect = ps_in.view_dir;
	
	float life = ps_in.clr.a;
	float4 clr = particle_tex.Sample(bilinear_sampler, ps_in.tex);
	
	return clr;
}

RWStructuredBuffer<float4> particle_pos_rw_stru_buff : register( t0 );
RWStructuredBuffer<float4> particle_vel_rw_stru_buff : register( t1 );

StructuredBuffer<float4> particle_init_vel_buff : register( t2 );
StructuredBuffer<float4> particle_birth_time_buff : register( t3 );

[numthreads(256, 1, 1)]
void cs_main(uint3 dtid : SV_DispatchThreadID)
{
	float4 out_pos;
	float4 out_vel;

	uint id = dtid.x;

	float4 cur_pos = particle_pos_rw_stru_buff[id];

	if (cur_pos.w > 0)
	{
		float3 cur_vel = particle_vel_rw_stru_buff[id].xyz;

		cur_vel += float3(0, -0.1f, 0) * elapse_time;
		cur_pos.xyz += cur_vel * elapse_time;
		cur_pos.w -= elapse_time;

		//float2 tex_pos = cur_pos.xz / 4 + 0.5;
		//tex_pos.y = 1 - tex_pos.y;
		//float height = height_map_tex.SampleLevel(point_sampler, tex_pos, 0).r;
		//if (cur_pos.y < height)
		//{
		//	cur_vel = reflect(cur_vel, decompress_normal(normal_map_tex.SampleLevel(normal_map_sampler, tex_pos, 0)).xzy) * 0.99f;
		//}

		out_pos = cur_pos;
		out_vel = float4(cur_vel, 1);
	}
	else
	{
		float t = accumulate_time - particle_birth_time_buff[id].r;
		if ((t > 0) && (t < elapse_time))
		{
			float4 pos = mul(float4(init_pos_life.xyz, 1), ps_model_mat);
			pos /= pos.w;

			float3 vel = particle_init_vel_buff[(id + uint((cur_pos.z * 256 + cur_pos.x) * 256)) & 0xFFFF].xyz;
			vel = mul(vel, (float3x3)ps_model_mat);

			out_pos = float4(pos.xyz, init_pos_life.w);
			out_vel = float4(vel, 1);
		}
		else
		{
			out_pos = float4(0, 0, 0, -1);
			out_vel = 0;
		}
	}

	particle_pos_rw_stru_buff[id] = out_pos;
	particle_vel_rw_stru_buff[id] = out_vel;
}
