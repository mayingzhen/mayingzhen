uniform mat4 matLightViewProj;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord0;
#ifdef SKIN
attribute vec4 a_blendIndices;
attribute vec4 a_blendWeights;
#endif

varying vec2 oUV;
varying vec4 oDepth;

void main()
{
#ifdef SKIN
	vec3 finalPos = SkinPos(a_position,a_blendIndices,a_blendWeights);
#else
	vec3 finalPos = a_position;
#endif

	vec4 worldPos = vec4(finalPos,1) * g_matWorld;

#ifdef WINDVERTEX
	WindVertex(g_matWorld, worldPos, a_position, a_normal);
#endif

	gl_Position = worldPos * matLightViewProj;

	oDepth = gl_Position;

	oUV = a_texCoord0;
#ifdef UVINV
	oUV.y = 1.0-oUV.y;
#endif
}



