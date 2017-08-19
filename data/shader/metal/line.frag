
#include "common.h"


// Varyings
struct PS_IN
{     
   float4   v_color;
  
};


fragment float4 ps_main(PS_IN In [[stage_in]])
{
	return In.v_color;
}
