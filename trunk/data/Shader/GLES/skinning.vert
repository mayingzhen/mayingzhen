vec4 _skinnedPosition;
#if defined(LIGHTING)
vec3 _skinnedNormal;
#endif


vec4 getPosition()
{
	_skinnedPosition = vec4(0.0);
 
    vec3 posOS = vec3(0.0,0.0,0.0);
	ivec4 iboneInd = ivec4(a_blendIndices);
   
	posOS += (u_matrixPalette[iboneInd.x] * vec4(a_position,1.0)).xyz * a_blendWeights.x;
	posOS += (u_matrixPalette[iboneInd.y] * vec4(a_position,1.0)).xyz * a_blendWeights.y;
	posOS += (u_matrixPalette[iboneInd.z] * vec4(a_position,1.0)).xyz * a_blendWeights.z;
	posOS += (u_matrixPalette[iboneInd.w] * vec4(a_position,1.0)).xyz * a_blendWeights.w;
	//posOS += (u_matrixPalette[iboneInd.w] * vec4(a_position,1.0)).xyz * (1.0 - a_blendWeights.x - a_blendWeights.y - a_blendWeights.z);
	
	_skinnedPosition = vec4(posOS,1.0);  

    return _skinnedPosition;    
}

#if defined(LIGHTING)

vec3 getTangentSpaceVector(vec3 vector)
{
	vector = vec3(0.0);
	
    ivec4 iboneInd = ivec4(a_blendIndices);
   
	vector += u_matrixPalette[iboneInd.x] * vector * a_blendWeights.x;
	vector += u_matrixPalette[iboneInd.y] * vector * a_blendWeights.y;
	vector += u_matrixPalette[iboneInd.z] * vector * a_blendWeights.z;
	vector += u_matrixPalette[iboneInd.w] * vector * a_blendWeights.w;
	
	retutn vector;
}

vec3 getNormal()
{
    return getTangentSpaceVector(a_normal);
}

#if defined(BUMPED)

vec3 getTangent()
{
    return getTangentSpaceVector(a_tangent);
}

vec3 getBinormal()
{
    return getTangentSpaceVector(a_binormal);
}

#endif
#endif