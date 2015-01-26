#ifdef OPENGL_ES
precision mediump float;
#endif

#include "common.h"
#include"shadowMap.h"

#ifdef COLORMAP
uniform sampler2D tColorMap;
#endif

#ifdef LIGHTMAP
uniform sampler2D tLightMap;
#endif

uniform sampler2D tBlendingMap;
uniform sampler2D tDetailMap0;
uniform sampler2D tDetailMap1;

varying vec2 oUV;
varying vec4 oWorldPos;
varying vec3 oWorldNormal;
#if USING_SHADOW != 0
varying vec4 oShadowPos;
#endif
#if USING_DECAL == 1
varying vec4 oProjPos;
#endif
#ifdef BLENDDATA
varying vec4 oBlend;
#endif

varying vec4 oColor;

varying vec4 oDetailUV;

uniform vec2 uBlendingOffset;
    
void main()
{
#ifdef BLENDDATA
	vec4 cBlending = oBlend;
#else
	vec4 cBlending = texture2D(tBlendingMap, oUV + uBlendingOffset);
#endif

#if LAYER==1
	gl_FragColor = texture2D(tDetailMap0, oDetailUV.xy);
#elif LAYER==2  
	vec4 cDetailMap0 = texture2D(tDetailMap0, oDetailUV.xy);
	vec4 cDetailMap1 = texture2D(tDetailMap1, oDetailUV.zw);
	gl_FragColor = cDetailMap0*cBlending.a + cDetailMap1*(1.0 - cBlending.a);
#endif

  vec4 cDiff = vec4(1.0,1.0,1.0,1.0);

#if USING_TERRAIN_COLORMAP != 0
#if defined(COLORDATA)
    cDiff *= iColor.rgb;	
#elif defined(COLORMAP)
    vec4 cColor = texture2D(tColorMap, oUV);
    cDiff.rgb *= cColor.rgb * (cColor.a*3.0);
#endif
#endif


#if USING_TERRAIN_LIGHTMAP != 0
#ifdef LIGHTMAP
	cDiff.rgb *= texture2D(tLightMap, vec2(oUV.x, 1.0-oUV.y)).rgb;
#endif
#endif

#ifdef OMNILIGHT   
	vec4 cOmni = ComputeOmniLight(oWorldPos.rgb, oWorldNormal.rgb);
	cDiff.rgb += cOmni.rgb;
#endif

	gl_FragColor.rgb *= cDiff.rgb;
  

#if USING_SHADOW != 0
	gl_FragColor.rgb *= DoShadowMapping(oShadowPos,oWorldPos.w);
#endif


#if USING_DECAL == 1
	gl_FragColor = AddDecal(oProjPos,gl_FragColor);
#endif

#if USING_FOG == 1
	gl_FragColor.rgb = CalcFog(oWorldPos.w, gl_FragColor.rgb);
#endif

	gl_FragColor.a = oColor.a;
}




