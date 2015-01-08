#ifdef POINT_LIGHT   
uniform float4 light_pos_es;
uniform	float4 light_radius;
#endif

#ifdef DIRECT_LIGHT
uniform float4 light_dir_es;
#endif

uniform float4 light_color ;//= (0.2f,0.2f,0.2f,0.2f);


uniform float4 depth_near_far_invfar;

// Gbuffer
sampler2D u_textureSceneDepth;
sampler2D u_textureSceneNormal;

// shadow
#ifdef SHADOW
sampler2D u_TextureSceneShadow;
#endif
      
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



float GetLinearDepth(float2 tc)
{
#ifdef HWDEPTH
	float q = depth_near_far_invfar.y / (depth_near_far_invfar.y - depth_near_far_invfar.x); 
	float depth = tex2D(u_textureSceneDepth, tc).r;
	depth = depth_near_far_invfar.x / (q - depth);
#else
	float depth = tex2D(u_textureSceneDepth, tc).r;
    depth *= depth_near_far_invfar.y;
#endif	
	
	return depth;
}

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

void GetPosNormalShiness(VS_OUT In,out float3 pos_es,out float3 normal,out float shiness)
{
   float depth = GetLinearDepth(In.oTc); 
   
   float3 view_dir = normalize(In.oViewDir);
   pos_es = view_dir * (depth / view_dir.z); 
   
   float4 SrcNormal = tex2D( u_textureSceneNormal, In.oTc);
   normal = decode(SrcNormal.xy);
   shiness = SrcNormal.w  * 255.0f;
}

void GetDiffuseSpecular(float3 lightVec, float3 pos_es, float3 normal,float shiness,out PS_OUT pOut)
{
   pOut = (PS_OUT)0;

   float3 vNormal = normalize(normal);
   float3 vLight  = normalize(lightVec);   
   float3 vView   = normalize(pos_es.xyz);
   float3 vHalfDir = normalize(vView + vLight);
   
   float3 light = lit( dot( vNormal, vLight ), dot( vNormal, vHalfDir ), shiness );   
   
   float3 cDiffUse = light.y * light_color;
   float3 cSpecular = light.z * light_color;

#ifdef POINT_LIGHT      
   float attenuation = saturate(1.0f - length(lightVec)/light_radius.x); 
#else 
   float attenuation = 1.0f; 
#endif        

   pOut.Diffuse.xyz = attenuation * cDiffUse;
   pOut.Specular.xyz = attenuation * cSpecular;  
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
   float3 vlightVec = light_dir_es.xyz;      
#endif
#endif  
   
   GetDiffuseSpecular(vlightVec,pos_es,normal,shiness,pOut);
}
#endif


void AmbientLightPS(VS_OUT In, out PS_OUT pOut)
{
   pOut = (PS_OUT)0;
   
   pOut.Diffuse.xyz = light_color;
   pOut.Specular.xyz = 0;   
}


void main( VS_OUT vout, out PS_OUT pout ) 
{
#ifdef AMBIENT_LIGHT
   AmbientLightPS(vout,pout);  
   return;
#else  

   DeferredLightPS(vout,pout);   
   
#ifdef SHADOW
   float shadow = tex2D(u_TextureSceneShadow, vout.oTc).r;
   
   pout.Diffuse *= shadow;
   pout.Specular *= shadow;
#endif

#endif
}

