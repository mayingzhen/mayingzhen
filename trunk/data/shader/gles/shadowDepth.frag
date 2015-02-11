#include "common.h"

uniform sampler2D   u_texture;

varying vec2 oUV;
varying vec4 oDepth;
 
vec4 OutPutDepth(float fDepth)
{
	fDepth = fDepth * 0.5 + 0.5 + 0.0013;

	return EncodeFloatRGBA(fDepth);
}

void main()
{
#ifdef ALPHATEST
	gl_FragColor = texture2D(u_texture, oUV);
	if(gl_FragColor.a < 0.5)
		discard;
#endif

	gl_FragColor = OutPutDepth(oDepth.z / oDepth.w);
}



