// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 

void main()
{
	float vignette_strength = 0.5;

	// Fetch sampler
	vec4 texture_color = texture2D( u_texture, v_uv );

	// Compute distance from the center (0.5, 0.5)
    float dist = distance(v_uv, vec2(0.5, 0.5));

	float vignette_factor = smoothstep(0.25, 0.5, dist);  // Adjust these values for the vignette's spread

    vec4 final_color = texture_color * (1.0 - vignette_factor * vignette_strength);

    // Assign the color to the pixel
	gl_FragColor = final_color;
}