

#ifdef IBL
TextureCube tEnv : register(t4);
SamplerState sEnv : register(s4);

Texture2D tBRDF : register(t5);
SamplerState sBRDF: register(s5);
#endif

#if defined(METAMAP) || defined(GLOSSINESSMAP)
Texture2D tSpecularRM;
SamplerState sSpecularRM;
	#if LAYER==2
		Texture2D tSpecularRM1;
		SamplerState sSpecularRM1;
	#endif
#endif


cbuffer ObjectLightPS : register(b2)
{
	float4 u_cDiffuseColor;

	float u_metalness;
	float u_glossiness;

	float2 u_diff_spec_mip;
};


void GetMetalnessGlossiness(float2 iUV, out float metalness, out float glossiness,float2 iUV1,float blendWeight)
{
#if defined(METAMAP) || defined(GLOSSINESSMAP)
	float2 specularRM = tSpecularRM.Sample(sSpecularRM,iUV.xy).rg;
	#if LAYER==2
		float2 specularRM1 = tSpecularRM1.Sample(sSpecularRM1,iUV1.xy).rg;
		specularRM = lerp( specularRM, specularRM1, blendWeight );
	#endif
#endif

	metalness = u_metalness;
#ifdef METAMAP
	metalness = specularRM.g;
#endif	

	glossiness = u_glossiness;
#ifdef GLOSSINESSMAP
	glossiness = specularRM.r;
#endif
}

// s2014_pbs_frostbite_slides.pptx
float SmoothToRoughness(float glossiness)
{
	return (1.0 - glossiness) * (1.0 - glossiness) /* (1.0 - glossiness)*/;
}

// GGX Distribution
// NdotH        = the dot product of the normal and the half vector
// roughness    = the roughness of the pixel
float GGXDistribution(float ndoth, float roughness)
{
	const float PI = 3.1415926f;

	float rough2 = roughness * roughness;
	float tmp =  (ndoth * rough2 - ndoth) * ndoth + 1;
	return rough2 / (PI * tmp * tmp);
}

// Blinn Distribution
// NdotH        = the dot product of the normal and the half vector
// roughness    = the roughness of the pixel
float BlinnPhongDistribution(float ndoth,float glossiness)
{
	float specPower = exp2(13.0f * glossiness);//pow(8192,glossiness);
	float Dpl = (specPower + 2) * 0.125 * pow(ndoth,specPower); // Blinn-Phong
	return Dpl;
}

float3 SchlichFresnel(float3 specColor,float ldoth)
{
	return specColor + (1.0 - specColor) * pow(1.0 - ldoth, 5.0);
}


//s2013_pbs_black_ops_2_slides_v2.pdf
float VisFun(float ndotl, float ndotv, float vdoth,float glossiness)
{
	float k = min(1.0,glossiness + 0.545);
	float Vlvh = 1.0 / ( k * vdoth * vdoth + 1.0 - k);
	return Vlvh;

	// Schlick-Smith Visibility Term
// 	float k = (0.8 + roughness * 0.5) * (0.8 + roughness * 0.5) / 2;
// 	float Gv = ndotv * (1 - k) + k;
// 	float Gl = ndotl * (1 - k) + k;
// 	return 1.0 / 4.0 * max( Gv * Gl, 1e-6 );

// 	float a = roughness * roughness;
// 	float Vis_SmithV = ndotl * ( ndotv * ( 1 - a ) + a );
// 	float Vis_SmithL = ndotv * ( ndotl * ( 1 - a ) + a );
// 	// Note: will generate NaNs with Roughness = 0.  MinRoughness is used to prevent this
// 	return 0.5 / ( Vis_SmithV + Vis_SmithL );
}

float3 LightBRDF(float glossiness,
						 float3 LightColor, float3 LightDir,
						 float3 vWorldNormal,float3 vView,
						 float3 diffColor,float3 specColor,
						 float fShadowMapShadow,float ao)
{
	float3 finalLight = float3(0,0,0);

	float3 vlight = LightDir;
	float3 vHalf = normalize(vView + vlight);
	
	float ndoth = max(0,dot(vWorldNormal,vHalf));
	float ldoth = max(0,dot(vlight,vHalf));
	float ndotl = max(0,dot(vlight,vWorldNormal));
	float ndotv = max(0,dot(vWorldNormal, vView)); 
	float vdoth = max(0,dot(vView, vHalf)); 

	//diffColor += float3(0.001f,0.001f,0.001f);
	finalLight = diffColor * ndotl * LightColor.rgb * fShadowMapShadow * ao;
	
//#ifdef SPEC	
	float roughness = SmoothToRoughness(glossiness);
	float Dis = GGXDistribution(ndoth,roughness);
	float3 Flh = SchlichFresnel(specColor,ldoth); 
	float Vlvh = VisFun(ndotl,ndotv,vdoth,glossiness);
	float3 specLight =  (Dis * Flh * Vlvh) * ndotl * LightColor.rgb * fShadowMapShadow;
	finalLight += specLight;
//#endif  

	return finalLight;
}


#ifdef IBL
float3 GlobalIBL(float glossiness, float3 diffColor,float3 specColor,float3 vWorldNormal,float3 vView)
{
    float ndotv = max(0,dot(vWorldNormal, vView)); 
	float3 vRelfDir = -reflect(vView, vWorldNormal);

	float roughness = (1.0 - glossiness) /* (1.0 - glossiness)*/;

	float3 diffuseIBL = tEnv.SampleLevel(sEnv, vWorldNormal, u_diff_spec_mip.x).rgb;

	float mip = roughness * u_diff_spec_mip.y;

	float3 specColorIBL = tEnv.SampleLevel(sEnv, vRelfDir,mip).rgb;

	float3 envDiffuse = diffColor.rgb * diffuseIBL.rgb;

	float2 brdf = tBRDF.Sample(sBRDF,float2(ndotv,roughness)).xy;

	float3 envSpec = specColorIBL * (specColor * brdf.x + brdf.y);

	return (envDiffuse + envSpec)/*g_fIBLScale*/;	    
}
#endif

float3 ForwardPixelLighting(float metalness,float glossiness,float3 vWorldNormal,
							float3 vView,float3 albedo,float fShadowMapShadow,float ao)
{
	float3 finalLight = float3(0.0,0.0,0.0);

	float3 mianDirLightColor = g_cDirLight;
	float3 mianDirLightDir = g_vDirLight;

	float3 skyLightColor = g_cSkyLight;

	float3 diffColor = albedo.rgb * (1.0 - metalness);
	float3 specColor = lerp(0.04, albedo.rgb, metalness); 

	float3 MainLight = LightBRDF(glossiness,mianDirLightColor,mianDirLightDir,
		vWorldNormal,vView,diffColor,specColor,fShadowMapShadow,ao);

	finalLight = MainLight;

#ifndef IBL 
	finalLight += skyLightColor * albedo * ao;
#endif

// �ڶ�յ����⣨ֻ���������䣩
#ifdef DIRLIGHTING1
	float ndotl1 = clamp(dot(vWorldNormal, normalize(-vDirLight1)), 0, 1);
	finalLight += albedo * ndotl1 * cDirLight1.rgb*cDirLight1.a * ao;
#endif
	
#ifdef IBL
    finalLight += GlobalIBL(glossiness, diffColor,specColor,vWorldNormal,vView);
#endif

	return finalLight;
}


struct DRMRTOut
{
	float4 oColor   : SV_TARGET0;

#if DEFERREDSHADING == 1
	float4 oColorNormal : SV_TARGET1;
#endif
};

void InitMRTOutPut(inout DRMRTOut mrtOut)
{
	mrtOut.oColor = 0;

#if DEFERREDSHADING == 1
	mrtOut.oColorNormal = 0;
#endif
}

void EnCodeMRTOutPut(float metalness,
					float glossiness,
					float3 albedo,
					float3 vWorldNormal,
					out DRMRTOut mrtOut) 
{
	InitMRTOutPut(mrtOut);

#if DEFERREDSHADING == 1 && !defined(TRANSPAENT)

	mrtOut.oColor.rgb = albedo.rgb;

	mrtOut.oColorNormal.xyz = vWorldNormal.xyz * 0.5 + 0.5; 

	mrtOut.oColor.w = metalness;
	mrtOut.oColorNormal.w = glossiness;
#endif

}



