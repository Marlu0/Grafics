// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 

void main()
{
	// Fetch sampler
	vec4 texture_color = texture2D( u_texture, v_uv );

	// Calculation of brightness
	float grayscale = dot(texture_color.rgb, vec3(0.299, 0.587, 0.114));
	vec3 yellow_grayscale = vec3(grayscale, grayscale, 0.0);

    // Assign the color to the pixel
	gl_FragColor = vec4(yellow_grayscale, texture_color.a);
}