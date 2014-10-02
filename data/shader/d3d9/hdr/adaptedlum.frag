
float frame_delta;

sampler2D g_SamplerSrc;

sampler2D g_SamplerLastLum;


float4 main() : COLOR
{
   float fAdaptedLum = tex2D(g_SamplerLastLum, float2(0.5f, 0.5f));
   
   float fCurrentLum = tex2D(g_SamplerSrc, float2(0.5f, 0.5f));

   float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * (1 - pow(0.98f, 30 * frame_delta));
   
   return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}