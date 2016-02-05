#include "common.h"
#include "skin.h"


uniform vec3 pos_extent;
uniform vec3 pos_center;
uniform vec4 tc_extent_center;

// Attributes
attribute vec3 a_position; 

#ifdef DIFFUSE   
attribute vec2 a_texCoord0;
#endif

attribute vec4 a_normal;

#ifdef SKIN   
attribute vec4 a_blendIndices;
attribute vec4 a_blendWeights;      
#endif   
 

#ifdef COLOR   
attribute vec4 a_color0;
#endif


// Varyings
varying vec4 v_position;

#ifdef DIFFUSE    
varying vec2 v_texCoord;
#endif  
   
#ifdef COLOR      
varying vec4 v_color;
#endif







void main()
{     
	vec3 iPos = a_position * pos_extent + pos_center;
#ifdef DIFFUSE  	
	vec2 iUV  = a_texCoord0 * tc_extent_center.xy + tc_extent_center.zw;
#endif	
	vec3 iNormal = a_normal * 2.0 - 1.0;     
    
#ifdef SKIN
	iPos  =  SkinPos(iPos,a_blendIndices,a_blendWeights);
#endif

	gl_Position = vec4(iPos,1.0) * g_matWorldViewProj; 
     
       
#ifdef DIFFUSE      
    v_texCoord = iUV;
#endif
    
#ifdef COLOR    
    v_color = a_color0;
#endif
    
}
