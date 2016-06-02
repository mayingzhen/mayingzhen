

TextureCube tEnv : register(t4);
SamplerState sEnv : register(s4);

cbuffer ObjectPS : register(b5)
{
	float4 u_cDiffuseColor;
	float4 u_cSpecColor;

	float2 u_diff_spec_mip;
	float u_mip_bias;
	float u_roughness;
	float u_envroughness;// = log2(u_roughness) / 13; // log2(8192) == 13
}

float CalcPrefilteredEnvMip(float roughness, int num_mip)
{
	return num_mip - roughness * num_mip;
}

float3 CalcPrefilteredEnvVec(float3 normal, float3 view)
{
	return -reflect(view, normal);
}

float2 CalcEnvbrdfTerm(float roughness, float3 normal, float3 view)
{
	float n_dot_v = saturate(dot(normal, view));
	roughness = max(0.5f / 16, roughness);
	float2 env_brdf;
	float4 tmp = ((float4(-4.996914762f, 7.253111161f, -1.963867075f, -0.170416225f) * roughness
		+ float4(8.993475061f, -15.42473953f, 4.712593001f, 0.448102365f)) * roughness
		+ float4(-4.928234727f, 10.95286522f, -3.852980973f, -0.389332014f)) * roughness
		+ float4(0.846124834f, -1.596815751f, 1.005729748f, 0.113484128f);
	env_brdf.x = (((tmp.x * n_dot_v + tmp.y) * n_dot_v + tmp.z) * n_dot_v) + tmp.w;
	tmp = ((float4(-0.684077741f, 1.316163916f, -0.70869252f, 0.073264505f) * roughness
		+ float4(0.719315865f, -1.384932814f, 0.775880046f, -0.105818706f)) * roughness
		+ float4(-0.101034049f, 0.18680998f, -0.117919199f, 0.030727381f)) * roughness
		+ float4(0.001922126f, 0.006697305f, -0.015741592f, 0.007229544f);
	env_brdf.y = (((tmp.x * n_dot_v + tmp.y) * n_dot_v + tmp.z) * n_dot_v) + tmp.w;
	env_brdf = saturate(env_brdf);
	return env_brdf;
}

float3 CalcEnvSpecular(float3 prefiltered_env, float3 c_spec, float roughness, float3 normal, float3 view)
{
	float n_dot_v = saturate(dot(normal, view));
	roughness = max(0.5f / 16, roughness);
	float2 env_brdf;
	float4 tmp = ((float4(-4.996914762f, 7.253111161f, -1.963867075f, -0.170416225f) * roughness
		+ float4(8.993475061f, -15.42473953f, 4.712593001f, 0.448102365f)) * roughness
		+ float4(-4.928234727f, 10.95286522f, -3.852980973f, -0.389332014f)) * roughness
		+ float4(0.846124834f, -1.596815751f, 1.005729748f, 0.113484128f);
	env_brdf.x = (((tmp.x * n_dot_v + tmp.y) * n_dot_v + tmp.z) * n_dot_v) + tmp.w;
	tmp = ((float4(-0.684077741f, 1.316163916f, -0.70869252f, 0.073264505f) * roughness
		+ float4(0.719315865f, -1.384932814f, 0.775880046f, -0.105818706f)) * roughness
		+ float4(-0.101034049f, 0.18680998f, -0.117919199f, 0.030727381f)) * roughness
		+ float4(0.001922126f, 0.006697305f, -0.015741592f, 0.007229544f);
	env_brdf.y = (((tmp.x * n_dot_v + tmp.y) * n_dot_v + tmp.z) * n_dot_v) + tmp.w;
	env_brdf = saturate(env_brdf);
	return prefiltered_env * (c_spec * env_brdf.x + env_brdf.y);
}

float3 CalcEnvDiffuse(float3 prefiltered_env, float3 c_diff)
{
	return prefiltered_env * c_diff;
}

float3 PrefilteredDiffuseIBL(float3 c_diff, float3 normal)
{
	//float3 prefiltered_clr = decode_hdr_yc(TexCubeSampleLevel(skybox_Ycube_tex, skybox_sampler, normal, u_diff_spec_mip.x, 0).r,
	//	TexCubeSampleLevel(skybox_Ccube_tex, skybox_sampler, normal, u_diff_spec_mip.x, 0)).xyz;
	float3 prefiltered_clr = tEnv.SampleLevel(sEnv,normal,u_diff_spec_mip.x).xyz;
	return CalcEnvDiffuse(prefiltered_clr, c_diff);
}

float3 PBFittingPrefilteredSpecularIBL(float3 c_spec, float roughness, float3 normal, float3 view)
{
	float mip = CalcPrefilteredEnvMip(roughness, u_diff_spec_mip.y);
	float3 r = CalcPrefilteredEnvVec(normal, view);
	//float3 prefiltered_clr = decode_hdr_yc(TexCubeSampleLevel(skybox_Ycube_tex, skybox_sampler, r, mip, u_mip_bias).r,
	//	TexCubeSampleLevel(skybox_Ccube_tex, skybox_sampler, r, mip, u_mip_bias)).xyz;
	float3 prefiltered_clr = tEnv.SampleLevel(sEnv,r,mip/*,u_mip_bias*/).xyz;
	return CalcEnvSpecular(prefiltered_clr, c_spec, roughness, normal, view);
}

float3 fresnel_term_schlick(float3 light_vec, float3 halfway_vec, float3 c_spec)
{
	float e_n = saturate(dot(light_vec, halfway_vec));
	return c_spec > 0 ? c_spec + (1 - c_spec) * exp2(-(5.55473f * e_n + 6.98316f) * e_n) : 0;
}


float specular_normalize_factor(float roughness)
{
	return (roughness + 2) / 8;
}

float3 roughness_term(float3 halfway_vec, float3 normal, float roughness)
{
	return pow(max(dot(halfway_vec, normal), 0.0f), roughness);
}

float3 roughness_term(float3 c_spec, float3 light_vec, float3 halfway_vec, float3 normal, float roughness)
{
	return pow(max(dot(halfway_vec, normal), 0.0f), roughness) * fresnel_term_schlick(light_vec, halfway_vec, c_spec);
}

float3 specular_term(float3 halfway_vec, float3 normal, float roughness)
{
	return specular_normalize_factor(roughness) * roughness_term(halfway_vec, normal, roughness);
}

float3 specular_term(float3 c_spec, float3 light_vec, float3 halfway_vec, float3 normal, float roughness)
{
	return specular_normalize_factor(roughness) * roughness_term(c_spec, light_vec, halfway_vec, normal, roughness);
}


// diffuse only
float3 calc_brdf(float3 c_diff, float3 l, float3 n)
{
	return max(c_diff * dot(n, l), 0);
}
// diffuse + specular
float3 calc_brdf(float3 c_diff, float3 c_spec, float roughness, float3 l, float3 h, float3 n)
{
	return max((c_diff + specular_term(c_spec, l, h, n, roughness)) * dot(n, l), 0);
}
