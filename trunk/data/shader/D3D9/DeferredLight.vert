#ifdef AMBIENT_LIGHT
#define SCEERN_LIGHT
#endif

#ifdef DIRECT_LIGHT
#define SCEERN_LIGHT
#endif

#ifdef SCEERN_LIGHT
uniform float4x4 u_InvProjMatrix ;
#else
uniform float4x4 u_worldViewProjectionMatrix; 
uniform float4x4 u_worldViewMatrix; 
#endif
   
struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};



void main( float3 pos : POSITION,
                 out VS_OUT vOut)
{
#ifdef SCEERN_LIGHT
   vOut.pos = float4(pos.xyz ,1);
   vOut.oViewDir = mul(u_InvProjMatrix, float4(pos.xyz ,1)).xyz;
#else
   vOut.pos =  mul(u_worldViewProjectionMatrix, float4(pos.xyz ,1));
   vOut.oViewDir =  mul(u_worldViewMatrix, float4(pos.xyz ,1)).xyz;
#endif
   
   vOut.oTc = vOut.pos.xy / vOut.pos.w * 0.5f;
   vOut.oTc.y *= -1;
   vOut.oTc += 0.5f;
}








