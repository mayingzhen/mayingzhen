#include "common.h"

attribute vec3 a_position;
attribute vec2 a_texCoord0;
attribute vec2 a_texCoord1;
attribute vec4 a_blendWeights;
attribute vec4 a_color0;

#ifdef COLOR_MULTIPLY
uniform vec4 colorMultiply;
#endif

#ifdef BILLBOARDOFFSET
uniform float billboard_offset;
#endif

varying vec2 oUV;
varying vec4 oDiff;

#ifdef TEXTURE_BLEND         
varying vec4 oNextUV;
#endif 

void main()
{
    vec4 worldPos = vec4(a_position,1.0);
#ifdef BILLBOARDOFFSET
    #if BILLBOARDOFFSET == 2
        worldPos.xyz += vec3(g_matView[2][0],g_matView[2][1],g_matView[2][2])*billboard_offset;
    #else
        worldPos.xyz += normalize(g_vEyeWorldPos - worldPos.xyz)*billboard_offset;
    #endif
#endif

    gl_Position = worldPos*g_matViewProj;
    oUV = a_texCoord0;
    
    oDiff = a_color0.bgra;
    
#ifdef COLOR_MULTIPLY
    oDiff *= colorMultiply;
#endif

#ifdef TEXTURE_BLEND 
    oNextUV.xy = a_texCoord1;
    oNextUV.z = a_blendWeights.x;
    oNextUV.w = 1.0;
#endif
}