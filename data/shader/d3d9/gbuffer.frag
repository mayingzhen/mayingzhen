float4 shininess = 16.0f;

struct VS_OUT
{      
   float4 oPos : POSITION;
  
#ifndef HWDEPTH
     float4 oPos2 : TEXCOORD0;
#endif     
     //float2 oTex : TEXCOORD1;
     float3 oNormal : TEXCOORD2;   
};


struct PS_OUT
{
  	float4 normal : COLOR0;
#ifndef HWDEPTH 	
	float4 Pos : COLOR1;
#endif	
};

float2 encode(float3 normal)
{
   //return normalize(normal.xy) * sqrt(normal.z * 0.5 + 0.5);
   return( normal.x / (1.0f - normal.z), normal.y / (1.0f - normal.z) );
}

float3 decode(float2 n)
{
	float fTemp = n.x * n.x + n.y * n.y;
	
	return (2 * n.x / (1 + fTemp), 2 * n.y / (1 + fTemp), (-1 + fTemp) / (1 + fTemp));
//    float3 normal;
//    normal.z = dot(n, n) * 2 - 1;
//    normal.xy = normalize(n) * sqrt(1 - normal.z * normal.z);
//    return normal;
}

void main( VS_OUT vout, out PS_OUT pout ) 
{
	pout.normal.xy = encode( normalize(vout.oNormal) );
	pout.normal.z = 0;
	pout.normal.w = shininess.x / 255.0f;
  	//pout.normal = float4( (normalize(vout.oNormal) + 1) * 0.5, shininess.x / 255.0f );
  	
#ifndef HWDEPTH	
	pout.Pos = vout.oPos2.z;
#endif
}
