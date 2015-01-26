#include "common.h"
#include "skin.h"




// Attributes
attribute vec3 a_position; 

#ifdef DIFFUSE   
attribute vec2 a_texCoord0;
#endif

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
    // Get the vertex position
    vec3 iPos = a_position;
    
#ifdef SKIN
	iPos  =  SkinPos(a_position,a_blendIndices,a_blendWeights);
#endif

	gl_Position = vec4(iPos,1.0) * g_matWorldViewProj; 
     
       
#ifdef DIFFUSE      
    v_texCoord = a_texCoord0;
#endif
    
#ifdef COLOR    
    v_color = a_color0;
#endif
    
}
