#include "PrefilterCube.h"


float4 main(float2 tex : TEXCOORD0) : SV_Target
{
  return PrefilterCubeDiffusePS(tex);
}

