

// Attributes
attribute vec3 a_position; 
attribute vec2 a_texCoord0;

// Varyings
varying vec2 v_texCoord;


vec2 TexCoordFromPos(float4 pos)
{
	vec2 tex = pos.xy / 2.0;
	tex.y *= -1.0;
	tex += 0.5;
	return tex;
}

void main()
{
	gl_Position = vec4(a_position.x, a_position.y, 1.0, 1.0);
	v_texCoord = TexCoordFromPos(gl_Position);
}
