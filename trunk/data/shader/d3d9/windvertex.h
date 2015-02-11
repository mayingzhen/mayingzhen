

#define windAnimationParameters float4(0.2,0.2,0.2,0.2)
#define mainBendingParameters float4(0.02, 0.05, 4.0, 1.0)
#define detailBendingParameters float4(0.01, 0, 1, 1.0)

void WindVertex(float4x4 worldMatrix, in out float4 worldPosition, float3 position, float3 normal)
{
	float4 vertexColor = float4(1,1,1,1);
	float3 objectWorldPosition = float3(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
	float3 localPosition = worldPosition.xyz - objectWorldPosition;

	float2 windSpeed = windAnimationParameters.xy;
	float2 waveSpeed = windAnimationParameters.zw;

	//Main Bending

	float bendScale = mainBendingParameters.x;
	float bendVariation = mainBendingParameters.y;
	float bendFrequency = mainBendingParameters.z;
	
	//calculate main bending strength
	float mainBendingPhase = sin(g_fTime * bendFrequency * length(windSpeed) + dot(objectWorldPosition, 1));
	float variationPhase = (dot(cos(g_fTime), mainBendingPhase) * .25f + .5f);
	float bendStrength = bendScale + bendVariation * variationPhase * mainBendingPhase;
	
	//calculate vertex distance and bending factor
	float fLength = length(localPosition.xyz);
	float fBF = localPosition.z * bendStrength;
	
	//smooth bending factor and increase its nearby height limit.
	fBF += 1.0;
	fBF *= fBF;
	fBF = fBF * fBF - fBF;
	
	//displace position
	float3 vNewPos = localPosition;
	vNewPos.xy += windSpeed * fBF;

	//rescale - this keeps the plant parts from "stretching" by shortening the z (height) while they move about the xy.
	localPosition = normalize(vNewPos.xyz) * fLength;

	//Detail Bending

	//#ifdef DETAIL_BENDING
		//parameters
		float branchAmplitude = detailBendingParameters.x;
		float leafAmplitude = detailBendingParameters.y;
		float branchFrequency = detailBendingParameters.z;
		float leafFrequency = detailBendingParameters.w;

		//calculate different phases
		float fTreePhase = dot(position.xyz, 1);
		float fBranchPhase = vertexColor.g + fTreePhase;
		float fLeafPhase = dot(position.xyz, fBranchPhase);

		//make some waves
		float2 vWavesIn = waveSpeed * (g_fTime + float2(fLeafPhase * leafFrequency, fBranchPhase * branchFrequency ));
		float4 vWaves = frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) );

		//apply the triangle wave and smooth function
		float4 triangleWave = abs( frac( vWaves + 0.5 ) * 2.0 - 1.0 )*1.000001;
		vWaves = triangleWave * triangleWave * (3.0 - 2.0 * triangleWave);
		
		//calculate resulting wave strength for two dimensions
		float2 vWavesSum = vWaves.xz + vWaves.yw;  

		localPosition.xyz += vWavesSum.xxy * float3(vertexColor.r * leafAmplitude * normal.xy, vertexColor.b * branchAmplitude);
	//#endif

	worldPosition.xyz = objectWorldPosition + localPosition;
}
