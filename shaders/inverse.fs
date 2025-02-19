// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 

void main()
{
	// Fetch sampler
	vec4 texture_color = texture2D( u_texture, v_uv );

	// Invert the colors by subtracting from 1.0
    vec3 inverted_color = 1.0 - texture_color.rgb;
    
    // Assign the color to the pixel
    gl_FragColor = vec4(inverted_color, texture_color.a);
}