

TextureCube tEnv : register(t4);
SamplerState sEnv : register(s4);

Texture2D tBRDFTerm : register(t5);
SamplerState sBRDFTerm : register(s5);

cbuffer ObjectLightPS : register(b6)
{
	float4 u_cDiffuseColor;
	float4 u_cSpecColor;

	float2 u_diff_spec_mip;
	float u_mip_bias;

	float u_metalness;
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
	float NoV = dot(normal, view);

	return tBRDFTerm.Sample(sBRDFTerm,float2(NoV,roughness)).xy;

	/*
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
	*/

	// [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
	// Adaptation to fit our G term.
	const float4 c0 = { -1, -0.0275, -0.572, 0.022 };
	const float4 c1 = { 1, 0.0425, 1.04, -0.04 };
	float4 r = roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	float2 AB = float2( -1.04, 1.04 ) * a004 + r.zw;

#if !(ES2_PROFILE || ES3_1_PROFILE)
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	// In ES2 this is skipped for performance as the impact can be small
	// Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
	//AB.y *= saturate( 50.0 * SpecularColor.g );
#endif

	//return SpecularColor * AB.x + AB.y;
	return AB;
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


float3 ForwardLighing(float3 cDiffuse,float3 cSpecColor, float roughness, float metalness, float3 vView,float3 vNormal)
{
#if defined(DIRLIGHT) || defined(ENVREFLECT)
	float3 oColor = float3(0,0,0);
#else
	float3 oColor = cDiffuse;
#endif 	
	
#ifdef DIRLIGHT	
	float3 vlight = normalize(g_vDirLight.xyz);
	float3 halfVec = normalize(vView + vlight);

	#ifdef BRDF
		#ifdef SPEC
			oColor.rgb = calc_brdf(cDiffuse,cSpecColor,roughness,vlight,halfVec,vNormal);
		#else
			oColor.rgb = calc_brdf(cDiffuse,vlight,vNormal);
		#endif		
	#else
		float4 light = lit( dot( vNormal, vlight ), dot( vNormal, halfVec ), roughness );   
		float3 Diffuse = light.y * cDiffuse * g_cDirLight.xyz;
		float3 Specular = light.z * cSpecColor.xyz * g_cDirLight.xyz;			
		oColor.xyz = g_cSkyLight + Diffuse;	

		#ifdef SPEC			
			oColor.xyz += Specular;	
		#endif 
	#endif 
#endif  

#ifdef ENVREFLECT
	//float3 diffuseIBL = PrefilteredDiffuseIBL(cDiffuse.xyz, vNormal);
	float3 diffuseIBL = tEnv.SampleLevel(sEnv,vNormal,u_diff_spec_mip.x).xyz;

	float3 dielectricColor = float3(0.04, 0.04, 0.04);
	float3 diffColor = cDiffuse.rgb * (1.0 - metalness);
	float3 specColor = lerp(dielectricColor.rgb, cDiffuse.rgb, metalness); //* specularIntensity;
	
	float mip = roughness * u_diff_spec_mip.y;//CalcPrefilteredEnvMip(1.0 - roughness, u_diff_spec_mip.y);
	float3 r = normalize( -reflect(vView, vNormal) /*CalcPrefilteredEnvVec(vNormal, vView)*/ );
	float3 specColorIBL = tEnv.SampleLevel(sEnv,r,mip).xyz;

	float3 albedoByDiffuse = diffColor.rgb * diffuseIBL.rgb;

	float2 env_brdf = CalcEnvbrdfTerm(roughness,vNormal, vView);
	float3 envSpec = specColorIBL * (specColor * env_brdf.x + env_brdf.y);

	oColor.xyz += albedoByDiffuse + envSpec;	

	//return specColorIBL;
#endif

	return oColor;
}