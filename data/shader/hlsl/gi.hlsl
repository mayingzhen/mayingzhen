#include"common.h"
#include"depth.h"


struct VS_OUT
{
   float2 oTc : TEXCOORD0;
   float4 oNDC : TEXCOORD1;
   float4 pos : SV_POSITION;
};


void vs_main(
	float2 pos : POSITION,
	float2 iUV : TEXCOORD0,
    out VS_OUT vOut)
{
   vOut.pos = float4(pos.x, pos.y, 0.0, 1);
   vOut.oNDC = vOut.pos;
   vOut.oTc = iUV;
}


#define FLT_MAX			3.40282347E+38
#define PI				3.1415926535897932
#define PI_DOUBLE		6.2831853071795864
#define ONE_OVER_PI		0.3183098861837906
#define TRACE_DEPTH		5
#define NUM_OBJECTS		17
#define USE_SSAA		0 //1

// =======================================================================
//
// Scene objects
//
// =======================================================================

struct SceneObject
{
	int		type;		// 0 -> light, 1 -> plane, 2 -> sphere
	float4	params;
	float3	baseColor;
	float	roughness;
	bool	metal;
};

static const SceneObject objects[NUM_OBJECTS] =
{
	{ 0, float4(-2.0f, 3.8f, 2.0f, 0.5f),			float3(60, 60, 60),				0,		false },	// light (~600 lumen)
	{ 1, float4(0, 1, 0, 0),						float3(0.664f, 0.824f, 0.85f),	0.3f,	true },		// bottom
	{ 1, float4(-1, 0, 0, 2.0f),					float3(1, 0, 0),				1.0f,	false },	// right
	{ 1, float4(0, 0, -1, 2.35f),					float3(1, 1, 1),				0.9f,	false },	// back
	{ 1, float4(1, 0, 0, 2.0f),						float3(0, 1, 0),				1.0f,	false },	// left
	{ 1, float4(0, -1, 0, 4.0f),					float3(1, 1, 1),				0.9f,	false },	// top
	{ 1, float4(0, 0, 1, 5.0f),						float3(1, 1, 1),				0.9f,	false },	// front
	{ 2, float4(-1.3f, 0.5f, 0.5f, 0.5f),			float3(1.0f, 0.3f, 0.1f),		0.1f,	false },	// sphere1
	{ 2, float4(1.2f, 0.5f, 0.25f, 0.5f),			float3(0.1f, 0.3f, 1.0f),		0.1f,	false },	// sphere2
	{ 2, float4(0, 0.75f, 1.2f, 0.75f),				float3(1.022f, 0.782f, 0.344f),	0.2f,	true },		// sphere3

	{ 2, float4(-1.6875f, 0.25f, -0.85f, 0.25f),	float3(0.972f, 0.96f, 0.915f),	0.0f,	true },		// silver sphere 1
	{ 2, float4(-1.125f, 0.25f, -0.85f, 0.25f),		float3(0.972f, 0.96f, 0.915f),	0.16f,	true },		// silver sphere 2
	{ 2, float4(-0.5625f, 0.25f, -0.85f, 0.25f),	float3(0.972f, 0.96f, 0.915f),	0.32f,	true },		// silver sphere 3
	{ 2, float4(0, 0.25f, -0.85f, 0.25f),			float3(0.972f, 0.96f, 0.915f),	0.48f,	true },		// silver sphere 4
	{ 2, float4(0.5625f, 0.25f, -0.85f, 0.25f),		float3(0.972f, 0.96f, 0.915f),	0.64f,	true },		// silver sphere 5
	{ 2, float4(1.125f, 0.25f, -0.85f, 0.25f),		float3(0.972f, 0.96f, 0.915f),	0.80f,	true },		// silver sphere 6
	{ 2, float4(1.6875f, 0.25f, -0.85f, 0.25f),		float3(0.972f, 0.96f, 0.915f),	1.0f,	true }		// silver sphere 7
};

// =======================================================================
//
// Path tracing functions
//
// =======================================================================

float RayIntersectPlane(out float3 n, float4 p, float3 start, float3 dir)
{
	float u = (dir.x * p.x + dir.y * p.y + dir.z * p.z);
	float t = 0;

	n = p.xyz;

	if( u < -1e-5f )
	//if( abs(u) > 1e-5f )
		t = -(start.x * p.x + start.y * p.y + start.z * p.z + p.w) / u;

	return ((t > 0.0f) ? t : FLT_MAX);
}

float RayIntersectSphere(out float3 n, float3 center, float radius, float3 start, float3 dir)
{
	float3 stoc = start - center;

	float a = dot(dir, dir);
	float b = 2.0f * dot(stoc, dir);
	float c = dot(stoc, stoc) - radius * radius;
	float d = b * b - 4.0f * a * c;
	float t = 0;

	if( d > 0.0f )
		t = (-b - sqrt(d)) / (2.0f * a);

	n = normalize(start + t * dir - center);

	return ((t > 0.0f) ? t : FLT_MAX);
}

int FindIntersection(out float3 pos, out float3 norm, float3 raystart, float3 raydir)
{
	float3	bestn, n;
	float	t, bestt	= FLT_MAX;
	int		index		= NUM_OBJECTS;
	int		i;

	// find first object that the ray hits
	[unroll]
	for( i = 0; i < NUM_OBJECTS; ++i )
	{
		[branch]
		if( objects[i].type == 1 )
			t = RayIntersectPlane(n, objects[i].params, raystart, raydir);
		else
			t = RayIntersectSphere(n, objects[i].params.xyz, objects[i].params.w, raystart, raydir);

		if( t < bestt )
		{
			bestt	= t;
			bestn	= n;
			index	= i;
		}
	}

	if( index < NUM_OBJECTS )
	{
		pos = raystart + (bestt - 1e-3f) * raydir;
		norm = bestn;
	}

	return index;
}

float Random(float3 pixel, float3 scale, float seed)
{
	return frac(sin(dot(pixel + seed, scale)) * 43758.5453 + seed);
}

float3 UniformSample(float3 n, float3 pixel, float seed)
{
	float u = Random(pixel, float3(12.9898f, 78.233f, 151.7182f), seed);
	float v = Random(pixel, float3(63.7264f, 10.873f, 623.6736f), seed);

	float phi = 2 * PI * u;
	float costheta = v;
	float sintheta = sqrt(1 - costheta * costheta);

	float3 H;

	H.x = sintheta * cos(phi);
	H.y = sintheta * sin(phi);
	H.z = costheta;

	float3 up = ((abs(n.z) < 0.999) ? float3(0, 0, 1) : float3(1, 0, 0));
	float3 tangent = normalize(cross(up, n));
	float3 bitangent = cross(n, tangent);

	// PDF = 1.0 / (2 * PI);
	return tangent * H.x + bitangent * H.y + n * H.z;
}

float3 CosineSample(float3 n, float3 pixel, float seed)
{
	float u = Random(pixel, float3(12.9898f, 78.233f, 151.7182f), seed);
	float v = Random(pixel, float3(63.7264f, 10.873f, 623.6736f), seed);

	float phi = 2 * PI * u;
	float costheta = sqrt(v);
	float sintheta = sqrt(1 - costheta * costheta);

	float3 H;

	H.x = sintheta * cos(phi);
	H.y = sintheta * sin(phi);
	H.z = costheta;

	float3 up = ((abs(n.z) < 0.999) ? float3(0, 0, 1) : float3(1, 0, 0));
	float3 tangent = normalize(cross(up, n));
	float3 bitangent = cross(n, tangent);

	return tangent * H.x + bitangent * H.y + n * H.z;
}

float3 GGXSample(float3 n, float roughness, float3 pixel, float seed)
{
	float u = Random(pixel, float3(12.9898f, 78.233f, 151.7182f), seed);
	float v = Random(pixel, float3(63.7264f, 10.873f, 623.6736f), seed);

	float m = roughness * roughness;
	float m2 = m * m;

	float phi = 2 * PI * u;
	float costheta = sqrt((1 - v) / (1 + (m2 - 1) * v));
	float sintheta = sqrt(1 - costheta * costheta);

	float3 H;

	H.x = sintheta * cos(phi);
	H.y = sintheta * sin(phi);
	H.z = costheta;
	
	float3 up = ((abs(n.z) < 0.999) ? float3(0, 0, 1) : float3(1, 0, 0));
	float3 tangent = normalize(cross(up, n));
	float3 bitangent = cross(n, tangent);

	return tangent * H.x + bitangent * H.y + n * H.z;
}

// =======================================================================
//
// BRDF functions
//
// =======================================================================

float GGX(float ndoth, float roughness)
{
	float m = roughness * roughness;
	float m2 = m * m;
	float d = (ndoth * m2 - ndoth) * ndoth + 1.0;

	return m2 / max(PI * d * d, 1e-8f);
}

float Smith(float ndotl, float ndotv, float roughness)
{
	float a = roughness * 0.5f + 0.5f;
	float a2 = a * a * a * a;

	float lambda_v = (-1 + sqrt(a2 * (1 - ndotl * ndotl) / (ndotl * ndotl) + 1)) * 0.5f;
	float lambda_l = (-1 + sqrt(a2 * (1 - ndotv * ndotv) / (ndotv * ndotv) + 1)) * 0.5f;

	return rcp(1 + lambda_v + lambda_l);
}

float3 BRDF_Diffuse(SceneObject obj)
{
	// Lambert
	return (obj.metal ? 0 : (obj.baseColor * ONE_OVER_PI));
}

float3 BRDF_Specular(SceneObject obj, float3 l, float3 v, float3 n)
{
	float3 h = normalize(l + v);
	float3 f0 = (obj.metal ? obj.baseColor : 0.04f);

	float ldoth = saturate(dot(l, h));
	float ndoth = saturate(dot(n, h));
	float ndotl = saturate(dot(l, n));
	float ndotv = saturate(dot(v, n));

	float3 F = f0 + (1.0f - f0) * pow(1 - ldoth, 5.0f);
	float D = GGX(ndoth, obj.roughness);
	float G = Smith(ndotl, ndotv, obj.roughness);

	return max(0, (D * F * G) / (4 * ndotv));
}

float3 BRDF_Specular_GGX(SceneObject obj, float3 l, float3 v, float3 n)
{
	// GGX sampled <=> PDF = (D * ndoth) / (4 * vdoth)
	float3 h = normalize(l + v);
	float3 f0 = (obj.metal ? obj.baseColor : 0.04f);

	float ldoth = saturate(dot(l, h));
	float ndoth = saturate(dot(n, h));
	float ndotl = saturate(dot(l, n));
	float ndotv = saturate(dot(v, n));
	float vdoth = saturate(dot(v, h));

	float3 F = f0 + (1.0f - f0) * pow(1 - ldoth, 5.0f);
	float G = Smith(ndotl, ndotv, obj.roughness);

	return max(0, (F * G * vdoth) / (ndotv * ndoth));
}

float3 BRDF_Specular_Cosine(SceneObject obj, float3 l, float3 v, float3 n)
{
	// cosine sampled <=> PDF = costheta / PI
	float3 h = normalize(l + v);
	float3 f0 = (obj.metal ? obj.baseColor : 0.04f);

	float ldoth = saturate(dot(l, h));
	float ndoth = saturate(dot(n, h));
	float ndotl = saturate(dot(l, n));
	float ndotv = saturate(dot(v, n));

	float3 F = f0 + (1.0f - f0) * pow(1 - ldoth, 5.0f);
	float D = GGX(ndoth, obj.roughness);
	float G = Smith(ndotl, ndotv, obj.roughness);

	// multiply with PI later
	return max(0, (D * F * G) / (4 * ndotv * ndotl));
}

float3 TraceScene(float3 raystart, float3 raydir, float3 pixel)
{
	float3	direct		= 0;
	float3	indirect	= 1;
	float3	lightrad	= 0;
	float3	irrad;
	float3	fr_cos_over_pdf;
	float3	outray		= raydir;
	float3	inray;
	float3	fd, fs;
	float3	n, p		= raystart;
	float	ldotn;
	int		j, index;

	float3 B = 1;

	[loop]
	//for( j = 0; j < 1; ++j )
	for( j = 0; j < TRACE_DEPTH; ++j )
	{
		index = FindIntersection(p, n, p, outray);

		if( index >= NUM_OBJECTS )
			break;

		if( objects[index].type == 0 )
		{
			// hit light
			lightrad = objects[index].baseColor;

			if( j == 0 )
				direct = lightrad;

			break;
		}

		//irrad = CalculateDirectIrradiance(p, n, pixel);
		fd = BRDF_Diffuse(objects[index]);

		//fr_cos_over_pdf = fd;
		
		[branch]
		if( objects[index].metal )
		{
			float3 refl = outray - 2 * dot(outray, n) * n;

			inray = GGXSample(refl, objects[index].roughness, pixel, g_fTime + float(j));
			fr_cos_over_pdf = BRDF_Specular_GGX(objects[index], inray, -outray, n);
		}
		else
		{
			inray = CosineSample(n, pixel, g_fTime + float(j));
			fs = BRDF_Specular_Cosine(objects[index], inray, -outray, n);

			fr_cos_over_pdf = (fd + fs) * PI;
		}
		

		/*
		float ndotl = saturate(dot(inray, n));
		float3 A = (fd * ndotl) + fs;

		direct += (irrad * A * B);
		B *= fr_cos_over_pdf;
		*/

		indirect *= fr_cos_over_pdf;
		outray = inray;
	}

	//return direct; // + indirect;
	return indirect * lightrad;
}

float3 TraceSubPixel(float2 subNDC, float4 spos)
{
	float4 ndc_pos = float4(subNDC.x,subNDC.y,g_vCameraNearFar.x,1.0);
	float4 wpos = mul(ndc_pos, g_matViewProjInv);
	wpos /= wpos.w;

	float3 eyePos = g_vEyeWorldPos;
	float3 raydir = normalize(wpos.xyz - eyePos);

	return TraceScene(eyePos, raydir, spos.xyz);
}



void ps_main(float2 iUV      : TEXCOORD0,
		float3 NDC : TEXCOORD1,
		float4 spos	    : SV_Position,
        out float4 oColor   :SV_TARGET)
{
	int samples_per_pixel = 200;
	float3 trace_color = 0;

	for( int i = 0; i < samples_per_pixel; ++i )
	{
		float u = Random(spos.xyz, float3(12.9898f, 78.233f, 151.7182f), g_fTime + float(i));
		float v = Random(spos.xyz, float3(63.7264f, 10.873f, 623.6736f), g_fTime + float(i));

		float2 uv = float2(u, v) * g_vViewport.zw;

		float2 subNDC = NDC.xy + uv;

		float4 _spos = spos;
		_spos.xy += float2(u, v);

		trace_color += TraceSubPixel(subNDC, _spos);
	}
	trace_color = trace_color / samples_per_pixel;

	oColor = float4(trace_color,1.0);
}

