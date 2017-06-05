#ifdef OPENGL_ES
precision mediump float;
#endif

#include"common.h"


uniform sampler2D tDeviceDepthMapSampler;

varying vec2 iUV;

void main()
{
	gl_FragColor = vec4(0.0,0.0,0.0,1.0);

	float q = g_vCameraNearFar.y / (g_vCameraNearFar.y - g_vCameraNearFar.x); 
	float depth = texture2D(tDeviceDepthMapSampler, iUV).r;
	depth = g_vCameraNearFar.x / (q - depth);
	gl_FragColor.r = depth * g_vCameraNearFar.z;
}


