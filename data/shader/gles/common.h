uniform mat4 g_matView;
uniform mat4 g_matViewInv;
uniform mat4 g_matProj;
uniform mat4 g_matViewProj;
uniform mat4 g_matWorld;
uniform mat4 g_matWorldView;
uniform mat4 g_matWorldViewProj;
uniform vec4 g_color;
uniform vec3 g_cSkyLight;
uniform vec3 g_cDirLight;
uniform vec3 g_vDirLight;
uniform vec3 g_vEyeWorldPos;
uniform vec2 g_vViewport;
uniform vec2 g_vViewportInv;

uniform float g_fTime;

vec3 transform_quat(vec3 v, vec4 quat)
{
	return v + cross(quat.xyz, cross(quat.xyz, v) + quat.w * v) * 2.0;
}

float RGBTOLUM(vec3 vRGB)
{
	return dot(vec3(0.27, 0.67, 0.06),vRGB);
}

                            
// Encoding a [0, 1) float into a rgba8. From http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final
vec4 EncodeFloatRGBA(float v)
{
	vec4 enc = vec4(1.0, 255.0, 65025.0, 16581375.0) * v;
	enc = fract(enc);
	enc -= enc.yzww * vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
	return enc;
}

float DecodeFloatRGBA(vec4 rgba)
{
	return dot(rgba, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0));
}
