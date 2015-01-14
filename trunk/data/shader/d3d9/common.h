float4x4 g_matView;
float4x4 g_matViewInv;
float4x4 g_matProj;
float4x4 g_matViewProj;
float4x4 g_matWorldView;
float4x4 g_matWorldViewProj;
float4x4 g_matWorld;
float4 g_color;
float3 g_cSkyLight;
float3 g_cDirLight;
float3 g_vDirLight;
float3 g_cSpecularLight;
float3 g_vEyeWorldPos;
float3 g_vAtWorldPos;
float2 g_vViewport;
float2 g_vViewportInv;
float4  g_vCameraNearFar;

float g_fTime;

// ---------------------------------------------------------------------
// Fog
// ---------------------------------------------------------------------
#if USING_FOG == 1
float2 g_fogRangeInv;
float3 g_fogFarColor;
float3 CalcFog(in float depth, in float3 color)
{
	float fFogPower = (depth - g_fogRangeInv.x)*g_fogRangeInv.y;
	return lerp(color, g_fogFarColor, clamp(fFogPower,0,1));
}
#endif

// ---------------------------------------------------------------------
// Warfog
// ---------------------------------------------------------------------
// <x,y>表示明暗图在XY平面的偏移，<z,w>表示明暗图在XY平面的尺寸
float4 g_rectWarfog;
float3 g_colorWarfog;
float3 g_warfogHeight_RangeInv_Scale;

// texture
texture mg_tWarfog;
sampler g_tWarfog
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
	string TextureAddressingMode = "TAM_BORDER,TAM_BORDER";
	string TextureBorderColour = "0.12 0.12 0.12 0.12";
> = sampler_state
{
    Texture = (mg_tWarfog);
};

//进行明暗图计算
float3 AddWarfog(in float4 inWorldPos, in float3 vOrigClr)
{
#if USING_WARFOG==1
	float fA = tex2D(g_tWarfog, (inWorldPos.xy-g_rectWarfog.xy)/g_rectWarfog.zw).w;
#if USING_WARFOG_HEIGHT == 1
	float factor = clamp((inWorldPos.z-g_warfogHeight_RangeInv_Scale.x)*g_warfogHeight_RangeInv_Scale.y, 0, 1);
	fA = lerp(fA, min(fA*g_warfogHeight_RangeInv_Scale.z, 1), factor);
#endif
	return (1-fA)*g_colorWarfog + fA*vOrigClr.xyz;
#else
    return vOrigClr;
#endif
}

float3 AddWarfogFor2(float4 inWorldPos, float3 vOrigClr)
{
#if USING_WARFOG==1
	float fA = tex2D(g_tWarfog, (inWorldPos.xy-g_rectWarfog.xy)/g_rectWarfog.zw).w;
#if USING_WARFOG_HEIGHT == 1
	float factor = clamp((inWorldPos.z-g_warfogHeight_RangeInv_Scale.x)*g_warfogHeight_RangeInv_Scale.y, 0, 1);
	fA = lerp(fA, min(fA*g_warfogHeight_RangeInv_Scale.z, 1), factor);
#endif
	return vOrigClr.xyz*fA;
#else
    return vOrigClr;
#endif
}

// ---------------------------------------------------------------------
// Decal
// ---------------------------------------------------------------------
// texture
texture mg_tDecalMap;
sampler g_tDecalMap
<
	string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
	string TextureAddressingMode = "TAM_BORDER,TAM_BORDER";
	string TextureBorderColour = "0.0 0.0 0.0 0.0";
> = sampler_state
{
    Texture = (mg_tDecalMap);
};

float4 AddDecal(float4 oProjPos, float4 vOrigClr)
{
#if USING_DECAL==1
    float2 uv = oProjPos.xy/oProjPos.w;
    uv.x = (uv.x + 1.0)*0.5;
    uv.y = (1.0-uv.y)*0.5;
    float4 oColor = tex2D( g_tDecalMap, uv);
    return oColor*oColor.w*2 + vOrigClr*(1-oColor.w);
#else
    return vOrigClr;
#endif
}

float4 AddDecal2(float4 oProjPos, float4 vOrigClr)
{
#if USING_DECAL==1
	float2 uv = oProjPos.xy/oProjPos.w;
    uv.x = (uv.x + 1.0)*0.5;
    uv.y = (1.0-uv.y)*0.5;
    float4 oColor = tex2D( g_tDecalMap, uv);
    return vOrigClr*(1-oColor.w);
#else
    return vOrigClr;
#endif
}

// ---------------------------------------------------------------------
// Terrain
// ---------------------------------------------------------------------
texture mtTerrainLightMap;
sampler2D g_tTerrainLightMap
<
string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
> = sampler_state
{
	Texture = (mtTerrainLightMap);
};

texture mtTerrainColorMap;
sampler2D g_tTerrainColorMap
<
string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
> = sampler_state
{
	Texture = (mtTerrainColorMap);
};

texture mtTerrainBlendingMap;
sampler2D g_tTerrainBlendingMap
<
string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
string TextureAddressingMode = "TAM_CLAMP,TAM_CLAMP";
> = sampler_state
{
	Texture = (mtTerrainBlendingMap);
};

#if USING_TERRAIN_CLOUD == 1
texture mtTerrainCloud;
sampler2D g_tTerrainCloud
<
string TextureFiltering = "FO_LINEAR,FO_LINEAR,FO_NONE";
string TextureAddressingMode = "TAM_WRAP,TAM_WRAP";
> = sampler_state
{
    Texture = (mtTerrainCloud);
};
#endif

float2 g_terrainStartpoint;
float2 g_terrainSize;

float2 CalcTerrainUV(float2 vWorldPos)
{
	return (vWorldPos - g_terrainStartpoint)/g_terrainSize;
}

float3 AddTerrainColorMap(float2 terrainUV)
{
#if USING_TERRAIN_COLORMAP == 1
	float4 terrainColor = tex2D(g_tTerrainColorMap, terrainUV);
	return terrainColor.rgb*(terrainColor.a*3.0);
#elif USING_TERRAIN_COLORMAP == 2
	float4 terrainBlendingMap = tex2D(g_tTerrainBlendingMap, terrainUV);
	return terrainBlendingMap.g*terrainBlendingMap.b*3.0;
#else
	return 1.0;
#endif
}

float AddTerrainLightMap(float2 terrainUV)
{
	return tex2D(g_tTerrainLightMap, float2(terrainUV.x, 1.0-terrainUV.y)).r;
}

#if USING_TERRAIN_CLOUD == 1
float AddTerrainCloud(float2 terrainUV)
{
    return tex2D(g_tTerrainCloud, terrainUV+float2(-0.002f,-0.002f)*g_fTime).r;
}
#endif



float4 pack(float f, float iRange)
{
    float4 fReturn = float4(0.0,0.0,0.0,0.0);
    float fRange = float(iRange);
    int i = int(f * iRange);
    fReturn.r = float(i) / fRange;
    f = (f - fReturn.r) * fRange;
    i = int(f * iRange);
    fReturn.g = float(i) / fRange;
    f = (f - fReturn.g) * fRange;
    i = int(f * iRange);
    fReturn.b = float(i) / fRange;
    fReturn.a = 1.0;
    return fReturn;
}

float unpack(float4 f, float iRange)
{
    float fRange = iRange;
    return f.r + f.g / fRange + f.b / (fRange * fRange);
}

// Encoding a [0, 1) float into a rgba8. From http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/
float4 EncodeFloatRGBA(float v)
{
	float4 enc = float4(1.0f, 255.0f, 65025.0f, 16581375.0f) * v;
	enc = frac(enc);
	enc -= enc.yzww * float4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 0);
	return enc;
}

float DecodeFloatRGBA(float4 rgba)
{
	return dot(rgba, float4(1, 1 / 255.0f, 1 / 65025.0f, 1 / 16581375.0f));
}


// ---------------------------------------------------------------------
// effect
// ---------------------------------------------------------------------
// useful constant
static float4x4 mClipToTex = float4x4(0.5f,	0.0f,	0.0f,	0.0f,
									 0.0f,	-0.5f,	0.0f,	0.0f,
									 0.0f,	0.0f,	0.0f,	0.0f,
									 0.5f,	0.5f,	0.0f,	1.0f);

// ---------------------------------------------------------------------
// omni light
// ---------------------------------------------------------------------
#if USING_OMNILIGHT==1
float4 g_omniLight[OMNI_COUNT];
float4 g_omniColor[OMNI_COUNT];
#endif

float4 ComputeOmniLight(float3 pixelPos, float3 pixelNor)
{
	float4 outColor = float4(0,0,0,0);

#if USING_OMNILIGHT==1
	int i=0;
	while(i<OMNI_COUNT)
	{
		float3 lightPos = g_omniLight[i].xyz;

		float3 dir = lightPos - pixelPos;
		float dis = length(dir);
		dir = normalize(dir);
		float factor = max(0, dot(pixelNor, dir));
		float3 lightcolor = g_omniColor[i].rgb;

		float range = g_omniLight[i].w;

		float disFactor = max(0, (1-dis/range));

		lightcolor *= (factor * disFactor);

		outColor.rgb += lightcolor;

		i++;
	}
	
#endif

    return outColor;
}

// ---------------------------------------------------------------------
// wind
// ---------------------------------------------------------------------
//float4 windAnimationParameters; //xy = windSpeed, zw = waveSpeed
//float4 mainBendingParameters;	//float4: bendScale, bendVariation, bendFrequency, UNUSED
//float4 detailBendingParameters;	//float4: branchAmplitude, detailAmplitude, branchFrequency, detailFrequency
#define windAnimationParameters float4(0.2,0.2,0.2,0.2)
#define mainBendingParameters float4(0.02, 0.05, 4.0, 1.0)
#define detailBendingParameters float4(0.01, 0, 1, 1.0)

void WindVertex(float4x4 worldMatrix, in out float4 worldPosition, float3 position, float3 normal)
{
	float4 vertexColor = float4(1,1,1,1);
	float3 objectWorldPosition = float3(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
	float3 localPosition = worldPosition.xyz - objectWorldPosition;

	float2 windSpeed = windAnimationParameters.xy;
	float2 waveSpeed = windAnimationParameters.zw;

	//Main Bending

	float bendScale = mainBendingParameters.x;
	float bendVariation = mainBendingParameters.y;
	float bendFrequency = mainBendingParameters.z;
	
	//calculate main bending strength
	float mainBendingPhase = sin(g_fTime * bendFrequency * length(windSpeed) + dot(objectWorldPosition, 1));
	float variationPhase = (dot(cos(g_fTime), mainBendingPhase) * .25f + .5f);
	float bendStrength = bendScale + bendVariation * variationPhase * mainBendingPhase;
	
	//calculate vertex distance and bending factor
	float fLength = length(localPosition.xyz);
	float fBF = localPosition.z * bendStrength;
	
	//smooth bending factor and increase its nearby height limit.
	fBF += 1.0;
	fBF *= fBF;
	fBF = fBF * fBF - fBF;
	
	//displace position
	float3 vNewPos = localPosition;
	vNewPos.xy += windSpeed * fBF;

	//rescale - this keeps the plant parts from "stretching" by shortening the z (height) while they move about the xy.
	localPosition = normalize(vNewPos.xyz) * fLength;

	//Detail Bending

	//#ifdef DETAIL_BENDING
		//parameters
		float branchAmplitude = detailBendingParameters.x;
		float leafAmplitude = detailBendingParameters.y;
		float branchFrequency = detailBendingParameters.z;
		float leafFrequency = detailBendingParameters.w;

		//calculate different phases
		float fTreePhase = dot(position.xyz, 1);
		float fBranchPhase = vertexColor.g + fTreePhase;
		float fLeafPhase = dot(position.xyz, fBranchPhase);

		//make some waves
		float2 vWavesIn = waveSpeed * (g_fTime + float2(fLeafPhase * leafFrequency, fBranchPhase * branchFrequency ));
		float4 vWaves = frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) );

		//apply the triangle wave and smooth function
		float4 triangleWave = abs( frac( vWaves + 0.5 ) * 2.0 - 1.0 )*1.000001;
		vWaves = triangleWave * triangleWave * (3.0 - 2.0 * triangleWave);
		
		//calculate resulting wave strength for two dimensions
		float2 vWavesSum = vWaves.xz + vWaves.yw;  

		localPosition.xyz += vWavesSum.xxy * float3(vertexColor.r * leafAmplitude * normal.xy, vertexColor.b * branchAmplitude);
	//#endif

	worldPosition.xyz = objectWorldPosition + localPosition;
}
