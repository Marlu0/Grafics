// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 


void main()
{
	// Fetch sampler
	vec4 texture_color = texture2D( u_texture, v_uv );

    // Assign the color to the pixel
	gl_FragColor = texture_color;
}