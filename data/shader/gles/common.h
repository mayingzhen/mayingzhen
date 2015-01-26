uniform mat4 g_matView;
uniform mat4 g_matViewInv;
uniform mat4 g_matProj;
uniform mat4 g_matViewProj;
uniform mat4 g_matWorld;
uniform mat4 g_matWorldView;
uniform mat4 g_matWorldViewProj;
uniform vec4 g_color;
uniform vec3 g_cSkyLight;
uniform vec3 g_cDirLight;
uniform vec3 g_vDirLight;
uniform vec3 g_cSpecularLight;
uniform vec3 g_vEyeWorldPos;
uniform vec3 g_vAtWorldPos;
uniform vec2 g_vViewport;
uniform vec2 g_vViewportInv;

uniform float g_fTime;

// ---------------------------------------------------------------------
// Fog
// ---------------------------------------------------------------------
#if USING_FOG == 1
uniform vec2 g_fogRangeInv;
uniform vec3 g_fogFarColor;
vec3 CalcFog(in float depth, in vec3 color)
{
    float fFogPower = (depth-g_fogRangeInv.x)*g_fogRangeInv.y;
    fogColor = mix(color, g_fogFarColor, clamp(fFogPower,0.0,1.0));
}
#endif

// ---------------------------------------------------------------------
// Warfog
// ---------------------------------------------------------------------
// <x,y>表示明暗图在XY平面的偏移，<z,w>表示明暗图在XY平面的尺寸
uniform sampler2D g_tWarfog;
uniform vec4 g_rectWarfog;
uniform vec3 g_colorWarfog;
uniform vec3 g_warfogHeight_RangeInv_Scale;
//进行明暗图计算
vec3 AddWarfog(in vec4 inWorldPos, in vec3 vOrigClr)
{
#if USING_WARFOG==1
    float fA = texture2D(g_tWarfog, (inWorldPos.xy-g_rectWarfog.xy)/g_rectWarfog.zw).w;
#if USING_WARFOG_HEIGHT == 1
    float factor = clamp((inWorldPos.z-g_warfogHeight_RangeInv_Scale.x)*g_warfogHeight_RangeInv_Scale.y, 0.0, 1.0);
    fA = mix(fA, min(fA*g_warfogHeight_RangeInv_Scale.z, 1.0), factor);
#endif
    return (1.0-fA)*g_colorWarfog + fA*vOrigClr.xyz;
#else
    return vOrigClr;
#endif
}

// ---------------------------------------------------------------------
// Decal
// ---------------------------------------------------------------------
uniform sampler2D g_tDecalMap;
vec4 AddDecal(vec4 oProjPos, vec4 vOrigClr)
{
#if USING_DECAL==1
    vec2 uv = oProjPos.xy/oProjPos.w;
    uv.x = (uv.x + 1.0)*0.5;
    uv.y = (uv.y+1.0)*0.5;
    vec4 oColor = texture2D( g_tDecalMap, uv);
    return oColor*oColor.w*2.0 + vOrigClr*(1.0-oColor.w);
#else
    return vOrigClr;
#endif
}

// ---------------------------------------------------------------------
// Terrain
// ---------------------------------------------------------------------
uniform sampler2D g_tTerrainColorMap;
uniform sampler2D g_tTerrainLightMap;
uniform sampler2D g_tTerrainBlendingMap;
uniform vec2 g_terrainStartpoint;
uniform vec2 g_terrainSize;
vec2 CalcTerrainUV(vec2 vWorldPos)
{
    return (vWorldPos - g_terrainStartpoint)/g_terrainSize;
}

vec3 AddTerrainColorMap(vec2 terrainUV)
{
#if USING_TERRAIN_COLORMAP == 1
    vec4 terrainColor = texture2D(g_tTerrainColorMap, terrainUV);
    return terrainColor.rgb*(terrainColor.a*3.0);
#elif USING_TERRAIN_COLORMAP == 2
    vec4 terrainBlendingMap = texture2D(g_tTerrainBlendingMap, terrainUV);
    return terrainBlendingMap.g*terrainBlendingMap.b*3.0;
#else
    return vec3(1.0,1.0,1.0);
#endif
}

float AddTerrainLightMap(vec2 terrainUV)
{
    return texture2D(g_tTerrainLightMap, vec2(terrainUV.x, 1.0-terrainUV.y)).r;
}

// ---------------------------------------------------------------------
// effect
// ---------------------------------------------------------------------
// useful constant
const mat4 mClipToTex = mat4(0.5,  0.0, 0.0, 0.5,
                              0.0, -0.5, 0.0, 0.5,
                              0.0,  0.0, 0.0, 0.0,
                              0.0,  0.0, 0.0, 1.0);
                              
// ---------------------------------------------------------------------
// wind
// ---------------------------------------------------------------------
//float4 windAnimationParameters; //xy = windSpeed, zw = waveSpeed
//float4 mainBendingParameters; //float4: bendScale, bendVariation, bendFrequency, UNUSED
//float4 detailBendingParameters;   //float4: branchAmplitude, detailAmplitude, branchFrequency, detailFrequency
#define windAnimationParameters vec4(0.2,0.2,0.2,0.2)
#define mainBendingParameters vec4(0.02, 0.05, 4.0, 1.0)
#define detailBendingParameters vec4(0.01, 0.0, 1.0, 1.0)

vec4 WindVertex(mat4 worldMatrix, vec4 worldPosition, vec3 position, vec3 normal)
{
    vec4 vertexColor = vec4(1.0,1.0,1.0,1.0);
    vec3 objectWorldPosition = vec3(worldMatrix[0][3], worldMatrix[1][3], worldMatrix[2][3]);
    vec3 localPosition = worldPosition.xyz - objectWorldPosition;

    vec2 windSpeed = windAnimationParameters.xy;
    vec2 waveSpeed = windAnimationParameters.zw;

    //Main Bending
    float bendScale = mainBendingParameters.x;
    float bendVariation = mainBendingParameters.y;
    float bendFrequency = mainBendingParameters.z;
    
    //calculate main bending strength
    float mainBendingPhase = sin(g_fTime * bendFrequency * length(windSpeed) + dot(objectWorldPosition, vec3(1.0,1.0,1.0)));
    float variationPhase = (dot(cos(g_fTime), mainBendingPhase) * 0.25 + 0.5);
    float bendStrength = bendScale + bendVariation * variationPhase * mainBendingPhase;
    
    //calculate vertex distance and bending factor
    float fLength = length(localPosition.xyz);
    float fBF = localPosition.z * bendStrength;
    
    //smooth bending factor and increase its nearby height limit.
    fBF += 1.0;
    fBF *= fBF;
    fBF = fBF * fBF - fBF;
    
    //displace position
    vec3 vNewPos = localPosition;
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
        float fTreePhase = dot(position.xyz, vec3(1.0, 1.0, 1.0));
        float fBranchPhase = vertexColor.g + fTreePhase;
        float fLeafPhase = dot(position.xyz, vec3(fBranchPhase,fBranchPhase,fBranchPhase));

        //make some waves
        vec2 vWavesIn = waveSpeed * (g_fTime + vec2(fLeafPhase * leafFrequency, fBranchPhase * branchFrequency ));
        vec4 vWaves = fract( vWavesIn.xxyy * vec4(1.975, 0.793, 0.375, 0.193) );

        //apply the triangle wave and smooth function
        vec4 triangleWave = abs( fract( vWaves + 0.5 ) * 2.0 - 1.0 );
        vWaves = triangleWave * triangleWave * (3.0 - 2.0 * triangleWave);
        
        //calculate resulting wave strength for two dimensions
        vec2 vWavesSum = vWaves.xz + vWaves.yw;  

        localPosition.xyz += vWavesSum.xxy * vec3(vertexColor.r * leafAmplitude * normal.xy, vertexColor.b * branchAmplitude);
    //#endif

    worldPosition.xyz = objectWorldPosition + localPosition;
    return worldPosition;
}

// Encoding a [0, 1) float into a rgba8. From http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final
vec4 EncodeFloatRGBA(float v)
{
	vec4 enc = vec4(1.0, 255.0, 65025.0, 16581375.0) * v;
	enc = fract(enc);
	enc -= enc.yzww * vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
	return enc;
}

float DecodeFloatRGBA(vec4 rgba)
{
	return dot(rgba, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0));
}
