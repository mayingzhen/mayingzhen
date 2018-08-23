

// Standard box filtering
float4 DownsampleBox4Tap(Texture2D tex, SamplerState samplerTex, float2 uv, float2 texelSize)
{
    float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0);

    float4 s;
    s =  tex.Sample(samplerTex, uv + d.xy);
    s += tex.Sample(samplerTex, uv + d.zy);
    s += tex.Sample(samplerTex, uv + d.xw);
    s += tex.Sample(samplerTex, uv + d.zw);

    return s * (1.0 / 4.0);
}

// Standard box filtering
float4 UpsampleBox(Texture2D tex, SamplerState samplerTex, float2 uv, float2 texelSize, float sampleScale)
{
    float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0) * (sampleScale * 0.5);

    float4 s;
    s =  tex.Sample(samplerTex, uv + d.xy);
    s += tex.Sample(samplerTex, uv + d.zy);
    s += tex.Sample(samplerTex, uv + d.xw);
    s += tex.Sample(samplerTex, uv + d.zw);

    return s * (1.0 / 4.0);
}

float Max3(float a, float b, float c)
{
    return max(max(a, b), c);
}

float4 QuadraticThreshold(float4 color, float threshold, float3 curve)
{
    // Pixel brightness
    float br = Max3(color.r, color.g, color.b);

    // Under-threshold part: quadratic curve
    float rq = clamp(br - curve.x, 0.0, curve.y);
    rq = curve.z * rq * rq;

    // Combine and apply the brightness response curve.
    color *= max(rq, br - threshold) / max(br, 0.0001);

    return color;
}


Texture2D _AutoExposureTex;
SamplerState sampler_AutoExposureTex;

Texture2D _MainTex;
SamplerState sampler_MainTex;

Texture2D _BloomTex;
SamplerState sampler_BloomTex;

float _SampleScale;
float4 _Threshold;
float4 _Params;

struct VaryingsDefault
{
    float4 vertex : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

float4 Prefilter(float4 color, float2 uv)
{
	//float autoExposure = _AutoExposureTex.Sample(sampler_AutoExposureTex, uv).r;
    //color *= autoExposure;
    color = min(_Params.x, color); // clamp to max
    color = QuadraticThreshold(color, _Threshold.x, _Threshold.yzw);
    return color;
}


float4 FragPrefilter4(VaryingsDefault i) : SV_Target
{
    float2 mainTexSize;
	_MainTex.GetDimensions(mainTexSize.x, mainTexSize.y);

	float4 color = DownsampleBox4Tap(_MainTex, sampler_MainTex, i.texcoord, 1.0f / mainTexSize);
    return Prefilter(color, i.texcoord);
}

float4 FragDownsample4(VaryingsDefault i) : SV_Target
{
    float2 mainTexSize;
	_MainTex.GetDimensions(mainTexSize.x, mainTexSize.y);

	float4 color = DownsampleBox4Tap(_MainTex, sampler_MainTex, i.texcoord, 1.0f / mainTexSize);
    return color;
}

float4 Combine(float4 bloom, float2 uv)
{
    float4 color = _BloomTex.Sample(sampler_BloomTex, uv);
    return bloom + color;
}

float4 FragUpsampleBox(VaryingsDefault i) : SV_Target
{
    float2 mainTexSize;
	_MainTex.GetDimensions(mainTexSize.x, mainTexSize.y);

	float4 bloom = UpsampleBox(_MainTex, sampler_MainTex, i.texcoord, 1.0f / mainTexSize, _SampleScale);
    return Combine(bloom, i.texcoord);
}

