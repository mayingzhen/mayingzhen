#include"common.h"
#include"depth.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------
Texture2D tDiff;
SamplerState sDiff;

Texture2D tDiff1;
SamplerState sDiff1;

Texture2D tDissolve;
SamplerState sDissolve;

Texture2D tProjectiveTexture;
SamplerState sProjectiveTexture;


// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------
cbuffer ObjectPS : register(b5)
{
	float glowFactor;
	float4 cSoftParamsRefractionPower;
	float4 projective_uvscale;

	float DissolveAmount;
	float DissolveTexScale;
	float4 DissolveColor;  
	float DissolveFadeSize;
};


void main(float2 oUV      : TEXCOORD0,
#if defined(SOFT_PARTICLE)|| defined(PROJECTIVE_TEXTURE)
		float4 oScreenTC : TEXCOORD1,
#endif	
#ifdef TEXTURE_BLEND  
		float4 oNextUV  : TEXCOORD2,
#endif		
        float4 oDiff : TEXCOORD3,
        
        out float4 oColor   :SV_TARGET)
{
    oColor = tDiff.Sample(sDiff, oUV);
    
#ifdef TEXTURE_BLEND
	float BlendFactor = oNextUV.z;
	float4 nextColor = tDiff.Sample(sDiff, oNextUV.xy);
	oColor = oColor * (1.0 - BlendFactor) + BlendFactor * nextColor;
#endif

#ifdef DISSOLVE
	float ClipTex = tDissolve.Sample(sDiff, oUV * DissolveTexScale).r; 
	float ClipAmount = ClipTex - DissolveAmount; 
	clip(ClipAmount + DissolveFadeSize);	
	float t = saturate(ClipAmount / -DissolveFadeSize);
	oColor = lerp(oColor, DissolveColor, t);
#endif

#ifdef DIFF1
    oColor *= tDiff1.Sample(sDiff1, oUV);
#endif

	oColor *= oDiff;

#if defined(SOFT_PARTICLE) || defined(PROJECTIVE_TEXTURE)
	float2 vProj = oScreenTC.xy / oScreenTC.w;
#endif

#if USING_MRT_POS == 1 || USING_DEPTHMAP == 1	
#ifdef SOFT_PARTICLE
	float2 cSoftParticleParams = cSoftParamsRefractionPower.xy;
    float depthMapDepth = GetLinearDepth(vProj);
    float delta_z = (depthMapDepth - oScreenTC.z);
    float lerp = smoothstep( cSoftParticleParams.x, cSoftParticleParams.y, delta_z );
    oColor.a = lerp * oColor.a;
#endif
#endif

#ifdef PROJECTIVE_TEXTURE
    oColor *= tProjectiveTexture.Sample(sProjectiveTexture, vProj*projective_uvscale.zw + projective_uvscale.xy);
#endif

#ifdef GLOW
    oColor *= glowFactor;
#endif
}
 

