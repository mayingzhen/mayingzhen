struct PS_IN
{
	float2 uv : TEXCOORD0;
};

uniform Texture2D gTex_Lum : register (t0);
uniform SamplerState gSam_Lum : register (s0);

uniform Texture2D gTex_Scene : register (t1);
uniform SamplerState gSam_Scene : register (s1);

uniform Texture2D gTex_Bright0 : register (t2);
uniform SamplerState gSam_Bright0 : register (s2);

uniform Texture2D gTex_Bright1 : register (t3);
uniform SamplerState gSam_Bright1 : register (s3);

uniform Texture2D gTex_Bright2 : register (t4);
uniform SamplerState gSam_Bright2 : register (s4);

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

float4 main(PS_IN In) : SV_TARGET
{
	float adaptedLum = gTex_Lum.Sample(gSam_Lum, float2(0.5, 0.5)).r;
	float3 texColor = gTex_Scene.Sample(gSam_Scene, In.uv).rgb;
	float3 texBright0 = gTex_Bright0.Sample(gSam_Bright0, In.uv).rgb * gBloomWeight.x;
	float3 texBright1 = gTex_Bright1.Sample(gSam_Bright1, In.uv).rgb * gBloomWeight.y;
	float3 texBright2 = gTex_Bright2.Sample(gSam_Bright2, In.uv).rgb * gBloomWeight.z;
	
	float3 texBright = texBright0 + texBright1 + texBright2;
	
	//texColor += texBright;

	//texColor = BlurShift(texColor, adaptedLum);
	
	texColor *= 1.0;//gExposure / (adaptedLum + 0.001f);

	float3 curr = Uncharted2Tonemap(texColor);
	
	const float W = 11.2;
	
	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	float3 color = curr*whiteScale;
	
	//color += texBright;
	
	float3 retColor = pow(color,1/2.2);
	
	return float4(retColor,1);
}



