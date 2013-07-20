float4 light_pos_es;
float4 light_dir_es;
float4 light_color ;//= (0.2f,0.2f,0.2f,0.2f);

float4 depth_near_far_invfar;

// Gbuffer
sampler2D u_TextureSrcPos;
sampler2D u_TextureSrcNormal;

// shadow
sampler2D u_TextureShadow;

      
struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};

struct PS_OUT
{
    float4 Diffuse  : COLOR0;
   float4 Specular  : COLOR1;
};


void GetPosNormalShiness(VS_OUT In,out float3 pos_es,out float3 normal,out float shiness)
{
   float depth = tex2D(u_TextureSrcPos, In.oTc).x;
   depth *= depth_near_far_invfar.y;
   
   float3 view_dir = normalize(In.oViewDir);
   pos_es = view_dir * (depth / view_dir.z);   
   
   float4 SrcNormal = tex2D( u_TextureSrcNormal, In.oTc);
   normal = SrcNormal.xyz * 2 - 1;
   shiness = SrcNormal.w  * 255.0f;
}

void GetDiffuseSpecular(float3 lightVec, float3 pos_es, float3 normal,float shiness,out PS_OUT pOut)
{
   pOut = (PS_OUT)0;

   float3 vLight  = normalize(lightVec);   
   float3 vView   = normalize(-pos_es.xyz);
   float3 halfDir = normalize(vView + vLight);
   
   float3 light = lit( dot( vLight, normal ), dot( halfDir, normal ), shiness );   
   
   float3 cDiffUse = light.y * light_color;
   float3 cSpecular = light.z * light_color;
      
   pOut.Diffuse.xyz = cDiffUse;
   pOut.Specular.xyz = cSpecular;   
}


#ifndef AMBIENT_LIGHT
void DeferredLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   float3 pos_es;
   float3 normal;
   float shiness;
   GetPosNormalShiness(In,pos_es,normal,shiness);

#ifdef POINT_LIGHT   
   float3 vlightVec = light_pos_es.xyz - pos_es.xyz;
#else
   #ifdef DIRECT_LIGHT
   float3 vlightVec = light_dir_es;
   #endif         
#endif   
   
   GetDiffuseSpecular(vlightVec,pos_es,normal,shiness,pOut);
}
#endif


void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   half3 cDiffuse = 0.1f;
   half3 cSpecular = 0.0f;
      
   pOut.Diffuse.xyz = cDiffuse;
   pOut.Specular.xyz = cSpecular;   
}


void main( VS_OUT vout, out PS_OUT pout ) 
{
#ifdef AMBIENT_LIGHT
   AmbientLightPS(vout,pout);  
#else
	DeferredLightPS(vout,pout);   
   
   #ifdef SHADOW
      half shadow = tex2D(u_SamplerShadow, In.oTc).r;
   
      pout.Diffuse *= shadow;
      pout.Specular *= shadow;
   #endif

#endif
         
}


