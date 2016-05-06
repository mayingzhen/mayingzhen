#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D tDiff;
#ifdef DIFF1
uniform sampler2D tDiff1;
#endif
uniform sampler2D tDissolve;

uniform float glowFactor;

uniform float DissolveAmount;
uniform float DissolveTexScale;
uniform vec4 DissolveColor;  
uniform float DissolveFadeSize;

varying vec2 oUV;
varying vec4 oDiff;
#ifdef TEXTURE_BLEND         
varying vec4 oNextUV;
#endif 
void main()
{
    gl_FragColor = texture2D(tDiff, oUV);
    
#ifdef DISSOLVE
    float ClipTex = texture2D(tDissolve, oUV.xy * DissolveTexScale).r; 
    float ClipAmount = ClipTex - DissolveAmount; 
    if (ClipAmount + DissolveFadeSize < 0.0)
    discard;	
    float t = clamp(ClipAmount / -DissolveFadeSize,0.0,1.0);
    gl_FragColor = gl_FragColor * (1.0 - t) + DissolveColor * t;
#endif              
    
#ifdef TEXTURE_BLEND
    float BlendFactor = oNextUV.z;
    vec4 nextColor = texture2D(tDiff, oNextUV.xy);
    gl_FragColor = mix(gl_FragColor, nextColor, BlendFactor);
#endif	

#ifdef DIFF1
    gl_FragColor *= texture2D(tDiff1, oUV);
#endif
	gl_FragColor *= oDiff;
  
#ifdef GLOW
	gl_FragColor *= glowFactor;
#endif
}