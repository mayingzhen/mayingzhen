// Uniforms
uniform mat4 u_worldViewProjectionMatrix;

#ifdef SKIN
uniform mat4 u_matrixPalette[SKIN_MATRIX_COUNT];
#endif


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




#ifdef SKIN
#include "skinning.vert"
#else
#include "skinning-none.vert" 
#endif


void main()
{    
    // Get the vertex position
    vec4 position = getPosition();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;
   
       
#ifdef DIFFUSE      
    v_texCoord = a_texCoord0;
#endif
    
#ifdef COLOR    
    v_color = a_color0;
#endif
    
}
