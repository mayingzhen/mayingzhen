
float4 main( 
	#ifndef HWPCF
	in float4 oDepth : TEXCOORD0
	#endif
	) : COLOR
{
#ifndef HWPCF
	return oDepth.z / oDepth.w;
#else
	return float4(0,0,0,0);
#endif
}


