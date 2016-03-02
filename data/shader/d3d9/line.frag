
#include "common.h"


// Varyings
struct PS_IN
{     
   float4   v_color : COLOR0;
  
};


void main(PS_IN In,
out float4 outColor : COLOR0 
) 
{
	outColor = In.v_color;  	
}
