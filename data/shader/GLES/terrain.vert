// transformations
uniform mat4 u_worldViewProjectionMatrix;

attribute vec3 a_position;
attribute vec2 a_texCoord0;
attribute vec2 a_texCoord1;



varying vec2 v_texCoord0;
varying vec2 v_texCoord1;


void main()
{
	gl_Position = u_worldViewProjectionMatrix * vec4(a_position, 1);
	
	v_texCoord0	= a_texCoord0;
	v_texCoord1 = a_texCoord1;
}




