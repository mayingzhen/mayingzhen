
float4 main( 
	#ifndef HWPCF
	in float4 oDepth : TEXCOORD0
	#endif
	) : COLOR
{
#ifndef HWPCF
	return oDepth.z / oDepth.w;
#else
	return 1.0f;
#endif
}


