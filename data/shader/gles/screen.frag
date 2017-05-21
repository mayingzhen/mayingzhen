
// Varyings
varying vec2 v_texCoord;

uniform sampler2D tSrcColor;

void main()
{
	gl_FragColor = texture2D(tSrcColor,v_texCoord);
}