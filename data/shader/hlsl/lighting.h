
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
