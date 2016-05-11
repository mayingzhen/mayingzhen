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

uniform sampler2D tBumpMap0;

uniform float specPower;
uniform float specMaterial;
uniform float g_heightScale;

varying vec2 oUV;
varying vec2 oUVOffset;
varying vec4 oWorldPos;
varying vec4 oNormal;
#ifdef BUMPMAP
varying vec3 oT;  
varying vec3 oB;  	
#endif

#if USING_SHADOW != 0
varying vec4 oShadowPos;
#endif

varying vec4 oDetailUV;

    
void main()
{
	vec4 cBlending = texture2D(tBlendingMap, oUVOffset);

#ifdef BUMPMAP
    mat3 obj_to_tangent;
    obj_to_tangent[0] = normalize(oT);
    obj_to_tangent[1] = normalize(oB);
    obj_to_tangent[2] = normalize(oNormal.xyz);
#endif

#if LAYER==1

  vec2 oUVPixel = oDetailUV.xy;
	#ifdef PARALLAXMAPPING 
        vec3 toEye = normalize(g_vEyeWorldPos.xyz - oWorldPos.xyz);
        float height = texture2D(tBumpMap0, oUVPixel).a;				//从alpha分量得到高度信息  
        height = (height - 1.0) * g_heightScale;            //高度倍增（向内）  
        mat3 W2T = transpose(obj_to_tangent);  
        vec3 toEyeTangent = toEye * W2T;                          //世界 -> 切线空间  
        vec2 offset = toEyeTangent.xy * height;                       //通过世界空间内坐标的offset获取纹理offset   
        oUVPixel += offset;                                       //纹理坐标偏移  
    #endif

    #ifdef BUMPMAP
        vec3 Normal = texture2D(tBumpMap0, oUVPixel).xyz;
    #endif

	gl_FragColor = texture2D(tDetailMap0, oUVPixel.xy);
#elif LAYER==2  
	vec4 cDetailMap0 = texture2D(tDetailMap0, oDetailUV.xy);
	vec4 cDetailMap1 = texture2D(tDetailMap1, oDetailUV.zw);
	gl_FragColor = cDetailMap0*cBlending.a + cDetailMap1*(1.0 - cBlending.a);
#endif

#ifdef BUMPMAP
	Normal = normalize(2.0 * Normal - 1.0);
    vec3 oWorldNormal = normalize( Normal * obj_to_tangent );
#else
    vec3 oWorldNormal = normalize(oNormal.xyz);
#endif


#if defined(LIGHTING)
	vec3 oL = normalize(g_vDirLight);
	float fNDotL = max( dot(oWorldNormal,oL) , 0);
	vec3 cDiff = g_cSkyLight + g_cDirLight * fNDotL;
	gl_FragColor.rgb *= cDiff.rgb;
#endif

#if USING_SHADOW != 0
	gl_FragColor.rgb *= DoShadowMapping(oShadowPos,oWorldPos.w);
#endif

	gl_FragColor.a = oNormal.w;
}




