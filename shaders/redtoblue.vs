varying vec2 v_uv;

void main()
{	
	// Remember the UV's range [0.0, 1.0]
	v_uv = gl_MultiTexCoord0.xy;

	gl_Position = vec4(gl_Vertex.x, gl_Vertex.y, 0.0, 1.0);
}