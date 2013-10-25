float4 depth_near_far_invfar;


float4x4 viwToLightProjArray[4];
float4 splitPos;

float4 u_shadowMapTexelSize;


#ifndef HWPCF
float4x4 wordLightView[4];
#endif

sampler2D SamplerShadowMap0;
sampler2D SamplerShadowMap1;
sampler2D SamplerShadowMap2;
sampler2D SamplerShadowMap3;

// Gbuffer
sampler2D u_textureSceneDepth;


struct VS_OUT
{
   float4 pos : POSITION;
   float2 oTc : TEXCOORD0;
   float3 oViewDir : TEXCOORD1;
};

float SamplePCF(sampler2D sam, float4 vTexCoord)
{
  // project texture coordinates
  vTexCoord.xyz /= vTexCoord.w;

  // 2x2 PCF Filtering
  // 
  float fShadow[4];
  fShadow[0] = (vTexCoord.z < tex2D(sam, vTexCoord + float2(                    0,                     0)));
  fShadow[1] = (vTexCoord.z < tex2D(sam, vTexCoord + float2(u_shadowMapTexelSize.y,                     0)));
  fShadow[2] = (vTexCoord.z < tex2D(sam, vTexCoord + float2(                    0, u_shadowMapTexelSize.y)));
  fShadow[3] = (vTexCoord.z < tex2D(sam, vTexCoord + float2(u_shadowMapTexelSize.y, u_shadowMapTexelSize.y)));

  float2 vLerpFactor = frac(u_shadowMapTexelSize.x * vTexCoord.xy);

  return lerp( lerp(fShadow[0], fShadow[1], vLerpFactor.x),
               lerp(fShadow[2], fShadow[3], vLerpFactor.x),
                    vLerpFactor.y);
}

float Calculate(sampler2D shadowMap,int index,float3 pos_es)
{
   float shadow = 1.0f;
   
   float4 projCoord = mul( viwToLightProjArray[index], float4( pos_es, 1 ) );
   
#ifdef HWPCF      
   shadow = tex2Dproj(shadowMap, projCoord);   
#else  
   shadow = SamplePCF(shadowMap, projCoord);       
#endif   

   return shadow;
}

void main( VS_OUT In,out float4 color : COLOR0 )
{
   float fDepth = tex2D(u_textureSceneDepth, In.oTc).x;
   fDepth *= depth_near_far_invfar.y;
   
   float3 view_dir = normalize(In.oViewDir);
   float3 pos_es = view_dir * (fDepth / view_dir.z);
   
   float4 temPos = splitPos;
   
   float shadow = 1.0f;
      
   if ( fDepth > temPos.x && fDepth < temPos.y )
   {
      shadow = Calculate(SamplerShadowMap0,0,pos_es);   
   }
   else if ( fDepth > temPos.y && fDepth < temPos.z)
   {
      shadow = Calculate(SamplerShadowMap1,1,pos_es);   
   }
   else if ( fDepth > temPos.z && fDepth < temPos.w )
   {
      shadow = Calculate(SamplerShadowMap2,2,pos_es);
   }
   else if ( fDepth > temPos.w )
   {
      shadow = Calculate(SamplerShadowMap3,3,pos_es);   
   }         
   
   color = shadow;
}






	


