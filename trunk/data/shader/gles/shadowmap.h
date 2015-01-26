
// texture
uniform sampler2D g_tShadowMap;

uniform mat4 g_matShadow[1];
uniform vec4 g_fSplitPlane;
uniform vec4 g_ShadowDepthFade;
uniform vec4 g_shadowMapTexelSize;


float GetDirShadowFade(float inLight, float depth)
{
	return clamp(inLight + clamp((depth - g_ShadowDepthFade.z) * g_ShadowDepthFade.w,0.0,1.0),0.0,1.0);
}

float SamplerDepth(sampler2D sam, vec4 vTexCoord)
{
	float fDepth = DecodeFloatRGBA( texture2DProj(sam, vTexCoord) );
	return fDepth;
}


float ShadowDepthCompare(sampler2D sam, vec4 vTexCoord)
{
	return SamplerDepth(sam,vTexCoord) > vTexCoord.z / vTexCoord.w ? 1.0 : 0.0;
}

float shadow_pcf(sampler2D sam, vec4 shadowPos)
{
	float fShadow0 = ShadowDepthCompare(sam, shadowPos);
	float fShadow1 = ShadowDepthCompare(sam, vec4(shadowPos.x + g_shadowMapTexelSize.y, shadowPos.yzw));
	float fShadow2 = ShadowDepthCompare(sam, vec4(shadowPos.x, shadowPos.y + g_shadowMapTexelSize.y, shadowPos.zw));
	float fShadow3 = ShadowDepthCompare(sam, vec4(shadowPos.xy + g_shadowMapTexelSize.yy, shadowPos.zw));

	return (fShadow0 + fShadow1 + fShadow2 + fShadow3) * 0.25;
}

void GetShadowPos(vec3 iWordPos,float fDistance,out vec4 shadowPos)
{
	shadowPos = vec4(0.0,0.0,0.0,1.0);

	//if (fDistance < g_fSplitPlane.x)
	{
		shadowPos =  vec4(iWordPos,1.0) * g_matShadow[0];
	}
}

float DoShadowMapping(vec4 vShadowPos,float fDistance)
{
	float fShadow = 1.0;

#if SHADOW_BLUR == 1
	fShadow = shadow_pcf(g_tShadowMap,vShadowPos);
#else
	fShadow = ShadowDepthCompare(g_tShadowMap, vShadowPos);
#endif

	fShadow = fShadow + 0.5;
	fShadow = GetDirShadowFade(fShadow,fDistance);


	return fShadow;
}






