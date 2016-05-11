#include"common.h"
#include"shadowMap.h"

uniform vec3 pos_extent;
uniform vec3 pos_center;
uniform vec4 tc_extent_center;

uniform vec2 uBlendingOffset;
uniform vec2 uCellAmount; 
uniform vec2 uDetailScale;
uniform vec4 uDetailOffSet;
uniform vec4 uDetailRot;
uniform float uCurMaterialID;

attribute vec4 a_position;
attribute vec2 a_texCoord0;
attribute vec4 a_tangent;

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
	vec3 iPos = a_position.xyz * pos_extent + pos_center;	
	vec2 iUV  = a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
	vec4 tangent_quat = a_tangent * 2.0 - 1.0; 
	float iMateriaID = a_position.w * 32767.5;
	
    oWorldPos = vec4(iPos,1.0);
    gl_Position = oWorldPos*g_matViewProj;
    
#if USING_SHADOW != 0
    GetShadowPos(oWorldPos.xyz,gl_Position.w,oShadowPos);
#endif

    oUV = iUV;
    
    oUVOffset = oUV + uBlendingOffset / uCellAmount.xy;
    
#if defined(BUMPMAP)
	oT = transform_quat(vec3(1.0, 0.0, 0.0), tangent_quat);
	oB = transform_quat(vec3(0.0, 1.0, 0.0), tangent_quat) * sign(tangent_quat.w);	
#endif
	oNormal.xyz = transform_quat(vec3(0.0, 0.0, 1.0), tangent_quat);    
    
    
    oDetailUV = vec4(0.0,0.0,0.0,0.0);

#if LAYER==1
    oDetailUV.xy = GetDetaiUV( oUV * uCellAmount, uDetailScale.x, uDetailRot.xy, uDetailOffSet.xy ); 
#elif LAYER==2
    oDetailUV.xy = GetDetaiUV( oUV * uCellAmount, uDetailScale.x, uDetailRot.xy, uDetailOffSet.xy ); 
    oDetailUV.zw = GetDetaiUV( oUV * uCellAmount, uDetailScale.y, uDetailRot.zw, uDetailOffSet.zw );
#endif
    
    oWorldPos.w = gl_Position.w;
   
  float fWeight = clamp(1 - abs(iMateriaID - uCurMaterialID),0.0,1.0);   	
  oNormal.w = fWeight;
	
}




