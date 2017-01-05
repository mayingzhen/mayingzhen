

TextureCube tEnv : register(t4);
SamplerState sEnv : register(s4);

Texture2D tBRDF : register(t5);
SamplerState sBRDF: register(s5);

Texture2D tSpecularRM;
SamplerState sSpecularRM;


cbuffer ObjectLightPS : register(b6)
{
	float4 u_cDiffuseColor;

	float u_metalness;
	float u_glossiness;

	float2 u_diff_spec_mip;
}


void GetMetalnessGlossiness(float2 oUV, out float metalness, out float glossiness)
{
	float2 specularRM = tSpecularRM.Sample(sSpecularRM,oUV.xy).rg;

	metalness = u_metalness;
#ifdef METAMAP
	metalness = specularRM.g;
#endif	

	glossiness = u_glossiness;
#ifdef GLOSSINESSMAP
	glossiness = specularRM.r;
#endif
}



// GGX Distribution
// NdotH        = the dot product of the normal and the half vector
// roughness    = the roughness of the pixel
float GGXDistribution(float ndoth, float roughness)
{
	float rough2 = roughness * roughness;
	float tmp =  (ndoth * rough2 - ndoth) * ndoth + 1;
	return rough2 / (tmp * tmp);
}

float3 LightBRDF(float glossiness,
				 float3 LightColor, float3 LightDir,
				 float3 vWorldNormal,float3 vView,
				 float3 diffColor,float3 specColor,
				 float fShadowMapShadow)
{
	float3 finalLight;

	float3 vlight = LightDir;
	float3 vHalf = normalize(vView + vlight);

	float ndoth = max(0,dot(vWorldNormal,vHalf));
	float ldoth = max(0,dot(vlight,vHalf));
	float ndotl = max(0,dot(vlight,vWorldNormal));
	float ndotv = max(0,dot(vWorldNormal, vView)); 
	float vdoth = max(0,dot(vView, vHalf)); 

	finalLight = diffColor * ndotl * LightColor.rgb * fShadowMapShadow;
	
#ifdef SPEC	
	float roughness = (1.0 - glossiness) * (1.0 - glossiness); 
	float Dis = GGXDistribution(ndoth,roughness);
	float3 Flh = specColor + (1.0 - specColor) * pow(1.0 - ldoth, 5.0); //Schlich-Fresnel
	float k = min(1.0,roughness + 0.545);
	float Vlvh = 1.0 / ( k * vdoth * vdoth + 1.0 - k);
	float3 specLight =  (Dis * Flh * Vlvh) * ndotl * LightColor.rgb * fShadowMapShadow;
	finalLight += specLight;
#endif  

	return finalLight;
}

float3 ForwardPixelLighting(float metalness,float glossiness,float3 vWorldNormal,
							float3 vView,float3 albedo,float fShadowMapShadow)
{
	float3 finalLight = float3(0.0,0.0,0.0);

	float3 mianDirLightColor = g_cDirLight;
	float3 mianDirLightDir = g_vDirLight;

	float3 skyLightColor = g_cSkyLight;

	float3 diffColor = albedo.rgb * (1.0 - metalness);
	float3 specColor = lerp(0.04, albedo.rgb, metalness); 

	float3 MainLight = LightBRDF(glossiness,mianDirLightColor,mianDirLightDir,
		vWorldNormal,vView,diffColor,specColor,fShadowMapShadow);

	finalLight = MainLight;

#ifndef IBL 
	finalLight += skyLightColor * albedo;
#endif

#ifdef IBL
	float ndotv = max(0,dot(vWorldNormal, vView)); 
	float3 vRelfDir = reflect(vView, vWorldNormal);	

	float roughness = (1.0 - glossiness) /* (1.0 - glossiness)*/;

	float3 diffuseIBL = tEnv.SampleLevel(sEnv, vWorldNormal,u_diff_spec_mip.x).rgb;

	float mip = roughness * u_diff_spec_mip.y;

	float3 specColorIBL = tEnv.SampleLevel(sEnv, vRelfDir,mip).rgb;

	float3 envDiffuse = diffColor.rgb * diffuseIBL.rgb;

	float2 brdf = tBRDF.Sample(sBRDF,float2(ndotv,roughness)).xy;

	float3 envSpec = specColorIBL * (specColor * brdf.x + brdf.y);

	finalLight += envDiffuse + envSpec;	
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

void FinalMRTOutPut(float metalness,
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