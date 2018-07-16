
float frame_delta;

Texture2D g_TexSrc;
SamplerState g_SamplerSrc;

Texture2D g_TexLastLum;
SamplerState g_SamplerLastLum;


float4 main() : SV_TARGET
{
   float fAdaptedLum = g_TexLastLum.Sample(g_SamplerLastLum, float2(0.5f, 0.5f)).r;
   
   float fCurrentLum = g_TexSrc.Sample(g_SamplerSrc, float2(0.5f, 0.5f)).r;

   float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * (1 - pow(0.98f, 30 * frame_delta));
   
   return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}