

cbuffer ObjectVS : register(b5)
{
	int face;
	float roughness;
};


TextureCube skybox_cube_tex : register(t1);
SamplerState skybox_sampler : register(s1);

float3 ToDir(int face, float2 xy)
{
	float3 dir;
	if (0 == face)
	{
		dir.x = +1;
		dir.yz = 1 - xy.yx * 2;
	}
	else if (1 == face)
	{
		dir.x = -1;
		dir.y = 1 - xy.y * 2;
		dir.z = xy.x * 2 - 1;
	}
	else if (2 == face)
	{
		dir.xz = xy.xy * 2 - 1;
		dir.y = +1;
	}
	else if (3 == face)
	{
		dir.x = xy.x * 2 - 1;
		dir.y = -1;
		dir.z = 1 - xy.y * 2;
	}
	else if (4 == face)
	{
		dir.x = xy.x * 2 - 1;
		dir.y = 1 - xy.y * 2;
		dir.z = +1;
	}
	else
	{
		dir.xy = 1 - xy.xy * 2;
		dir.z = -1;
	}

	return normalize(dir);
}

uint ReverseBits(uint bits)
{
	bits = (bits << 16) | (bits >> 16);
	bits = ((bits & 0x55555555) << 1) | ((bits & 0xAAAAAAAA) >> 1);
	bits = ((bits & 0x33333333) << 2) | ((bits & 0xCCCCCCCC) >> 2);
	bits = ((bits & 0x0F0F0F0F) << 4) | ((bits & 0xF0F0F0F0) >> 4);
	bits = ((bits & 0x00FF00FF) << 8) | ((bits & 0xFF00FF00) >> 8);
	return bits;
}

float RadicalInverseVdC(uint bits)
{
	return ReverseBits(bits) * 2.3283064365386963e-10f; // / 0x100000000
}

float2 Hammersley2D(uint i, uint N)
{
	return float2(float(i) / N, RadicalInverseVdC(i));
}

float3 TangentToWorld( float3 Vec, float3 TangentZ )
{
	float3 UpVector = abs(TangentZ.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
	float3 TangentX = normalize( cross( UpVector, TangentZ ) );
	float3 TangentY = cross( TangentZ, TangentX );
	return TangentX * Vec.x + TangentY * Vec.y + TangentZ * Vec.z;
}

float3 ImportanceSampleLambert(float2 xi)
{
	const float PI = 3.1415926f;

	float phi = 2 * PI * xi.x;
	float cos_theta = sqrt(1 - xi.y);
	float sin_theta = sqrt(1 - cos_theta * cos_theta);
	return float3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
}

float3 ImportanceSampleLambert(float2 xi, float3 normal)
{
	float3 h = ImportanceSampleLambert(xi);
	
	float3 up_vec = abs(normal.z) < 0.999f ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 tangent = normalize(cross(up_vec, normal));
	float3 binormal = cross(normal, tangent);
	return tangent * h.x + binormal * h.y + normal * h.z;
}

float3 ImportanceSampleBP(float2 xi, float roughness)
{
	const float PI = 3.1415926f;

	float phi = 2 * PI * xi.x;
	float cos_theta = pow(1 - xi.y * (roughness + 1) / (roughness + 2), 1 / (roughness + 1));
	float sin_theta = sqrt(1 - cos_theta * cos_theta);
	return float3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
}

float3 ImportanceSampleBP(float2 xi, float roughness, float3 normal)
{
	float3 h = ImportanceSampleBP(xi, roughness);
	
	float3 up_vec = abs(normal.z) < 0.999f ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 tangent = normalize(cross(up_vec, normal));
	float3 binormal = cross(normal, tangent);
	return tangent * h.x + binormal * h.y + normal * h.z;
}

float3 importanceSampleGGX(float2 xi, float roughness, float3 normal)
{
	const float PI = 3.1415926f;

	float a = roughness * roughness;

	float Phi = 2 * PI * xi.x;
	float CosTheta = sqrt((1 - xi.y) / (1 + (a*a - 1) * xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);

	float3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	float3 UpVector = abs(normal.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 TangentX = normalize(cross(UpVector, normal));
	float3 TangentY = cross(normal, TangentX);

	return TangentX * H.x + TangentY * H.y + normal * H.z;
}

float4 CopySrcPS(float2 tex : TEXCOORD0) : SV_Target
{
	float3 normal = ToDir(face, tex);

	return skybox_cube_tex.SampleLevel(skybox_sampler, normal, 0);
}



float4 PrefilterCubeDiffusePS(float2 tex : TEXCOORD0) : SV_Target
{
	float3 normal = ToDir(face, tex);

	float3 prefiltered_clr = 0;

	const uint NUM_SAMPLES = 1024;
	for (uint i = 0; i < NUM_SAMPLES; ++ i)
	{
		float2 xi = Hammersley2D(i, NUM_SAMPLES);
		float3 l = ImportanceSampleLambert(xi, normal);

		prefiltered_clr += skybox_cube_tex.SampleLevel(skybox_sampler, l, 0).xyz;
	}

	return float4(prefiltered_clr / NUM_SAMPLES, 1);
}


float4 PrefilterCubeSpecularPS(float2 tex : TEXCOORD0) : SV_Target
{
	float3 r = ToDir(face, tex);

	float3 normal = r;
	float3 view = r;
	float3 prefiltered_clr = 0;
	float total_weight = 0;

	const uint NUM_SAMPLES = 1024;
	for (uint i = 0; i < NUM_SAMPLES; ++ i)
	{
		float2 xi = Hammersley2D(i, NUM_SAMPLES);
		float3 h = importanceSampleGGX(xi, roughness, normal);
		float3 l = -reflect(view, h);
		float n_dot_l = saturate(dot(normal, l));
		if (n_dot_l > 0)
		{
			prefiltered_clr += skybox_cube_tex.SampleLevel(skybox_sampler, l, 0).xyz * n_dot_l;
			total_weight += n_dot_l;
		}
	}

	return float4(prefiltered_clr / max(1e-6f, total_weight), 1);
}


float GGX(float NoV, float roughness)
{
	// http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
	// Schlick-Beckmann G.
	float k = roughness/2;
	return NoV / (NoV * (1.0f - k) + k);
}

float geometryForLut(float roughness, float NoL)
{
	return GGX(NoL, roughness * roughness) ;
}

// Visibility term
float visibilityForLut(float roughness, float NoV)
{
	return GGX(NoV, roughness * roughness);
}

float4 IntegrateBRDF(float2 tex : TEXCOORD0) : SV_Target
{
	float roughness = tex.y;//(1.0 - tex.y) * (1.0 - tex.y);
	float n_dot_v = tex.x;

	float3 normal = float3(0.0f, 0.0f, 1.0f);
	float3 view = float3(sqrt(1.0f - n_dot_v * n_dot_v), 0.0f, n_dot_v);
	float2 rg = float2(0, 0);

	float Vis = visibilityForLut(roughness, n_dot_v);

	const uint NUM_SAMPLES = 1024;
	for (uint i = 0; i < NUM_SAMPLES; ++i)
	{
		float2 xi = Hammersley2D(i, NUM_SAMPLES);
		float3 h = importanceSampleGGX(xi, roughness, normal);
		float3 l = -reflect(view, h);
		float n_dot_l = clamp(l.z, 0.0f, 1.0f);
		float n_dot_h = clamp(h.z, 0.0f, 1.0f);
		float v_dot_h = clamp(dot(view, h), 0.0f, 1.0f);
		if (n_dot_l > 0)
		{
			//float g = GImplicit(n_dot_v, n_dot_l);
			//float g_vis = g * v_dot_h / std::max(1e-6f, n_dot_h * n_dot_v);
			//float fc = pow(1 - v_dot_h, 5);
			//rg += float2(1 - fc, fc) * g_vis;
			float g = geometryForLut(roughness, n_dot_l) * Vis;
			float g_vis = g * v_dot_h / (n_dot_h * n_dot_v);
			float fc = pow(1 - v_dot_h, 5);
			rg += float2(1 - fc, fc) * g_vis;
		}
	}

	return float4(rg / NUM_SAMPLES, 1, 1);
}


