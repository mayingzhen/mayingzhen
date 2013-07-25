#ifdef OPENGL_ES
precision highp float;
#endif

uniform sampler2D TerrainTex;


varying vec2 v_texCoord0;
varying vec2 v_texCoord1;


void main()
{    
	vec4 texColor0 = texture2D(TerrainTex, v_texCoord0); 
    vec4 texColor1 = texture2D(TerrainTex, v_texCoord1);
   
	vec3 texColor = texColor0.xyz + texColor1.a * (texColor1.xyz - texColor0.xyz);                                    
	gl_FragColor = vec4(texColor,texColor1.a);
}




