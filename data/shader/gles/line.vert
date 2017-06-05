#include "common.h"


uniform mat4 g_matWorldView;
uniform mat4 g_matWorldViewProj;
uniform mat4 g_matWorld;

// Attributes
attribute vec3 a_position;  
attribute vec4 a_color0;

// Varyings
varying vec4 v_position;
varying vec4 v_color;


void main()
{     
	vec3 iPos = a_position;

	gl_Position = vec4(iPos,1.0) * g_matWorldViewProj; 
     
    v_color = a_color0; 
}



