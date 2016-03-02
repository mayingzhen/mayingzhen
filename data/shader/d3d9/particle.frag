#include"common.h"
#include"depth.h"

// ---------------------------------------------------------------------
// sampler
// ---------------------------------------------------------------------
uniform sampler2D tDiff;
uniform sampler2D tDiff1;
uniform sampler2D tDissolve;
uniform sampler2D tProjectiveTexture;


// ---------------------------------------------------------------------
// local param
// ---------------------------------------------------------------------
float glowFactor;
float4 cSoftParamsRefractionPower;
float4 projective_uvscale;

float DissolveAmount;
float DissolveTexScale;
float4 DissolveColor;  
float DissolveFadeSize;


void main(float2 oUV      : TEXCOORD0,
#if defined(SOFT_PARTICLE)|| defined(PROJECTIVE_TEXTURE)
		float4 oScreenTC : TEXCOORD1,
#endif	
#ifdef TEXTURE_BLEND  
		float4 oNextUV  : TEXCOORD2,
#endif		
        float4 oDiff : TEXCOORD3,
        
        out float4 oColor   :COLOR0)
{
    oColor = tex2D(tDiff, oUV);
    
#ifdef TEXTURE_BLEND
	float BlendFactor = oNextUV.z;
	float4 nextColor = tex2D(tDiff, oNextUV.xy);
	oColor = oColor * (1.0 - BlendFactor) + BlendFactor * nextColor;
#endif

#ifdef DISSOLVE
	float ClipTex = tex2D (tDissolve, oUV * DissolveTexScale).r; 
	float ClipAmount = ClipTex - DissolveAmount; 
	clip(ClipAmount + DissolveFadeSize);	
	float t = saturate(ClipAmount / -DissolveFadeSize);
	oColor = lerp(oColor, DissolveColor, t);
#endif

#ifdef DIFF1
    oColor *= tex2D(tDiff1, oUV);
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
    oColor *= tex2D(tProjectiveTexture, vProj*projective_uvscale.zw + projective_uvscale.xy);
#endif

#ifdef GLOW
    oColor *= glowFactor;
#endif
}
 

