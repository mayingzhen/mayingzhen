#ifdef OPENGL_ES
precision highp float;
#endif

#ifdef DIFFUSE   
uniform sampler2D u_texture;
#endif

// Varyings
#ifdef DIFFUSE
varying   vec2 v_texCoord;
#endif
   
#ifdef COLOR      
varying   vec4 v_color;
#endif   


#ifdef DIFFUSECOLOR   
uniform vec4 u_cDiffuseColor; 
#endif

void main()
{
#ifdef DIFFUSECOLOR   
	gl_FragColor = u_cDiffuseColor;
#else
	gl_FragColor = vec4(1.0,1.0,1.0,1.0);
#endif	
	
#ifdef COLOR   
	gl_FragColor = v_color;   
#endif

#ifdef  DIFFUSE
	gl_FragColor *= texture2D(u_texture, v_texCoord);

#endif
	
}