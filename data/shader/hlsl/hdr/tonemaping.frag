struct PS_IN
{
	float2 uv : TEXCOORD0;
};

uniform Texture2D gTex_Lum : register (t0);
uniform SamplerState gSam_Lum : register (s0);

uniform Texture2D gTex_Scene : register (t1);
uniform SamplerState gSam_Scene : register (s1);

uniform Texture2D gTexBloom : register (t2);
uniform SamplerState gSamBloom : register (s2);

uniform float gExposure;
uniform float4 gBloomWeight;

//const float A = 0.15;
// const float B = 0.50;
// const float C = 0.10;
// const float D = 0.20;
// const float E = 0.02;
// const float F = 0.30;
// const float W = 11.2;

float3 Uncharted2Tonemap(float3 x)
{
const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;


  return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

float3 BlurShift(float3 vSample, float fLum)
{
	static const float3 LUMINANCE_VECTOR  = float3(0.2125f, 0.7154f, 0.0721f);
	static const float3 BLUE_SHIFT_VECTOR = float3(1.05f, 0.97f, 1.27f); 
	float fMaxBlueShiftCoefficient = 0.2f;

	float fBlueShiftCoefficient = 1.0f - (fLum + 1.5)/4.1;

    fBlueShiftCoefficient = saturate(fBlueShiftCoefficient);
	fBlueShiftCoefficient = lerp(0, fMaxBlueShiftCoefficient, fBlueShiftCoefficient);

	// Lerp between current color and blue, desaturated copy
    float3 vRodColor = dot(vSample, LUMINANCE_VECTOR) * BLUE_SHIFT_VECTOR;
        
	return lerp(vSample, vRodColor, fBlueShiftCoefficient) ;
}

float3 ToneMap_Hejl2015(float3 hdr)
{
	float WhitePoint_Hejl = 11.2;
    float4 vh = float4(hdr, WhitePoint_Hejl);
    float4 va = (1.435 * vh) + 0.05;
    float4 vf = ((vh * va + 0.004) / ((vh * (va + 0.55) + 0.0491))) - 0.0821;
    return vf.xyz / vf.www;
}


float4 main(PS_IN In) : SV_TARGET
{
	float avgLuminance = gTex_Lum.Sample(gSam_Lum, float2(0.5, 0.5)).r;
	avgLuminance = max(avgLuminance, 0.001);

	float keyValue = 0.12;
	//keyValue = 1.03 - 2.0 / ( log2(avgLuminance + 1.0) + 2.0 );

	float ExposureValue = keyValue / avgLuminance;
	//ExposureValue = gExposure;

	float3 texColor = gTex_Scene.Sample(gSam_Scene, In.uv).rgb;
	float3 texBright0 = gTexBloom.Sample(gSamBloom, In.uv).rgb ;//* gBloomWeight.x;

	texColor += texBright0;

	float3 color = texColor;//ToneMap_Hejl2015(texColor * ExposureValue);

	
	//color += texBright;
	
	//float3 retColor = pow(color,1/2.2);
	
	return float4(color,1);
}





