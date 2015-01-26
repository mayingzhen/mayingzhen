#include"common.h"
#include"shadowMap.h"

uniform vec2 uCellAmount; 
uniform vec2 uDetailScale;
uniform vec4 uDetailOffSet;
uniform vec4 uDetailRot;
uniform float uCurMaterialID;

attribute vec3 a_position;
attribute vec2 a_texCoord0;
attribute vec4 a_normal;
attribute vec3 a_tangent;
attribute vec4 a_color0;
attribute vec4 a_color1;

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


vec2 GetDetaiUV(vec2 oUVPixel,float fDetailScale, vec2 fDetailRot, vec2 uDetailOffSet)
{
	vec2 uv = oUVPixel * fDetailScale;

#ifdef DETAILROTATE
	float rsin = fDetailRot.x;
	float rcos = fDetailRot.y;
	mat2 rotateP = mat2( rcos, rsin, -rsin, rcos );
	uv = uv * rotateP;
#endif

	 uv += uDetailOffSet;

	 return uv;
}

void main()
{
    oWorldPos = vec4(a_position,1.0);
    gl_Position = oWorldPos*g_matViewProj;
    
#if USING_SHADOW != 0
    GetShadowPos(oWorldPos.xyz,gl_Position.w,oShadowPos);
#endif

    oUV = a_texCoord0;
    
    oDetailUV = vec4(0.0,0.0,0.0,0.0);

#if LAYER==1
    oDetailUV.xy = GetDetaiUV( oUV * uCellAmount, uDetailScale.x, uDetailRot.xy, uDetailOffSet.xy ); 
#elif LAYER==2
    oDetailUV.xy = GetDetaiUV( oUV * uCellAmount, uDetailScale.x, uDetailRot.xy, uDetailOffSet.xy ); 
    oDetailUV.zw = GetDetaiUV( oUV * uCellAmount, uDetailScale.y, uDetailRot.zw, uDetailOffSet.zw );
#endif
    
    oWorldPos.w = gl_Position.w;
    
#if USING_DECAL == 1
     oProjPos = gl_Position;
#endif
    oWorldNormal = a_normal.xyz;

  oColor.rgb = a_color0;
  float fWeight = clamp(1 - abs(a_color0.a * 255.0 - uCurMaterialID),0.0,1.0);   	
  oColor.a = fWeight;
		
#ifdef BLENDDATA
  oBlend = a_color1;
#endif
}




