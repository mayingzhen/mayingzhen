#include"common.h"
#include"depth.h"

cbuffer ObjectVS : register(b5)
{
	float4 colorMultiply;
	float billboard_offset;
}

void main(float4 iPos     : POSITION0,
        float2 iUV		: TEXCOORD0,
        float2 iNextUV	: TEXCOORD1,
        float4 iBlend	: BLENDWEIGHT0,
        float4 vc       : COLOR0,
        
        // output
        out float2 oUV      : TEXCOORD0,
#if defined(SOFT_PARTICLE) || defined(PROJECTIVE_TEXTURE)
		out float4 oScreenTC : TEXCOORD1,
#endif	
#ifdef TEXTURE_BLEND         
        out float4 oNextUV  : TEXCOORD2,
#endif      
        out float4 oDiff : TEXCOORD3,
        out float4 oPos     : SV_POSITION
)
{
    float4 worldPos = float4(iPos.xyz,1.0);
#ifdef BILLBOARDOFFSET
    #if BILLBOARDOFFSET == 2
        worldPos.xyz += g_matView._m02_m12_m22*billboard_offset;
    #else
        worldPos.xyz += normalize(g_vEyeWorldPos - worldPos.xyz)*billboard_offset;
    #endif
#endif

    oPos = mul(worldPos, g_matViewProj);
    oUV = iUV;

#if defined(SOFT_PARTICLE) || defined(PROJECTIVE_TEXTURE)
    float4 vHPos = oPos;         		  
	vHPos.y = -vHPos.y;
    oScreenTC.xy = (vHPos.xy + vHPos.w) * 0.5;    
    oScreenTC.zw =  float2(oPos.w, vHPos.w); 
#endif 
    
#ifdef TEXTURE_BLEND 
    oNextUV.xy = iNextUV;
	oNextUV.z = iBlend.b;
	oNextUV.w = 1.0;
#endif		
	
	oDiff = vc;
	
#ifdef COLOR_MULTIPLY
	oDiff *= colorMultiply;
#endif
}


 


