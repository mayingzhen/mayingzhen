
#include "common.h"


// Varyings
struct PS_IN
{     
   float4   v_color : TEXCOORD0;
  
};


fragment void main(PS_IN In [[stage_in]],
out float4 outColor : SV_TARGET 
) 
{
	outColor = In.v_color;  	
}
